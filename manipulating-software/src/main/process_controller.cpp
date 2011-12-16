/**
 * This file is part of crystal-orientation-data-collection.
 * 
 * crystal-orientation-data-collection is free software: you can
 * redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your
 * option) any later version.
 * 
 * crystal-orientation-data-collection is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with crystal-orientation-data-collection. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <wx/wfstream.h> 
#include <wx/msgdlg.h> 
#include <wx/fileconf.h> 
#include <wx/tokenzr.h> 

#include "main/process_controller.h"
#include "main/datafile_monitor.h"
#include "main/datafile_storage.h"
#include "main/macro_scheduler.h"
#include "main/confirm_dialog.h"
#include "main/app.h"


ProcessController * ProcessController::m_pInstance = NULL;

ProcessController::ProcessController()
{
	m_confirm_dialog = NULL ;
	
	// start the file transfer daemon
	DataFileStorage::Get()->Start() ;
}

ProcessController::~ProcessController()
{

	if(m_confirm_dialog!=NULL)
	{
		wxDynamicCast(m_confirm_dialog, ConfirmDialog)->Destroy();
	}
}

/**
 * Return the instance
 * Singleton pattern.
 */
ProcessController * ProcessController::Get()
{
	if( ! m_pInstance)
	{
		m_pInstance = new ProcessController;
	}
	return m_pInstance ;
}

wxFileName ProcessController::GetCurrentSessionDirName()
{
	wxFileName session_dir ;
	
	if(m_current_session_id != wxEmptyString)
	{
		session_dir.AssignDir(wxT("sessions")) ;
		session_dir.AppendDir(m_current_session_id);
	}
	return session_dir;
}

/**
 * Get the setting for current session from the METADATA file
 */
wxString ProcessController::ReadSessionMetaData(const wxString & key)
{
	wxFileName cfg_filename = GetCurrentSessionDirName();
	cfg_filename.SetName(SESSION_FILENAME_METADATA) ;
	cfg_filename.SetEmptyExt();
	
	wxFileInputStream cfg_stream(cfg_filename.GetFullPath());
	if(cfg_stream.IsOk())
	{
		wxFileConfig metadata_config(cfg_stream);
		// enumeration variables
		wxString str;

		if(metadata_config.Read(wxT("metadata/") + key, &str))
		{
			return str ;
		}
	}
	
	return wxEmptyString ;

}

/*
 * Present a dialog for confirmation before starting a new session
 */
void ProcessController::ConfirmNewSession(const wxString & equipment_id)
{
	if(m_confirm_dialog==NULL)
	{
		// Display the session confirm dialog
		ConfirmDialog * confirm_dialog = new ConfirmDialog();
		m_confirm_dialog = wxDynamicCast(confirm_dialog, wxObject) ;
	}
	wxDynamicCast(m_confirm_dialog, ConfirmDialog)->ConfirmNewSession(equipment_id) ;
}


/*
 * Start a new session
 */
void ProcessController::StartNewSession(const wxString & exchange_file)
{
	wxGetApp().Log(_T("Starting session with exchange file ") + exchange_file);

	// TODO: STOP CURRENT SESSION FIRST

	/**
	 * Create Session directory structure
	 */
	wxString session_id ;
	wxFileName session_filename("sessions", wxDateTime::Now().Format(FILENAME_DATETIME_FORMAT), "ini") ;

	// session has been assigned an unique id
	if( !session_filename.DirExists() )
	{
		wxGetApp().Log(wxString::Format(wxT("Creating dir %s") , session_filename.GetPath()));
		session_filename.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
	}

	m_current_session_id = session_id ;

	wxCopyFile(exchange_file, session_filename.GetFullPath());

	// END Create Session Ddirectory structure 

	LaunchEquipment();
	StartMonitoring();
}



bool ProcessController::LaunchEquipment()
{
	wxString equipment_id = ReadSessionMetaData(wxT("session.equipment.id"));

	bool launch_enabled = wxFileConfig::Get()->ReadBool (
			wxT("equipment.")
			+ equipment_id
			+ wxT(".launcher.enabled"),
			false
		) ;

	if( launch_enabled )
	{
		
	}
	else
	{
		wxString launch_program = wxFileConfig::Get()->Read(
				wxT("equipment.")
				+ equipment_id
				+ wxT(".launcher.program")
			) ;
		wxGetApp().Log(wxT("Starting program ") + launch_program);
		wxFileName launch_file(launch_program);
		long pid = wxExecute (launch_file.GetFullPath());
		wxGetApp().Log(wxString::Format("Program PID %d", pid));

		return pid != 0 ;
		
	}

	return false;
}




bool ProcessController::StartMonitoring()
{
	// TODO: Start Monitor files here
	DataFileMonitor * monitor = DataFileMonitor::Get() ;
	monitor->Reset();
	
	wxString equipment_id = ReadSessionMetaData(wxT("session.equipment.id"));


	// Set the monitored extensions
	{
		wxString exts = wxFileConfig::Get()->Read(
				wxString::Format(wxT("equipment.%s.monitor.extensions"), equipment_id)
			) ;

		wxStringTokenizer tokenizer(exts, wxT(", "));
		while ( tokenizer.HasMoreTokens() )
		{
			monitor->AddExtension( tokenizer.GetNextToken() ) ;
		}
	}


	// Set the monitored folders
	{
		wxString folders = wxFileConfig::Get()->Read(
				wxString::Format(wxT("equipment.%s.monitor.directories"), equipment_id)
			) ;

		wxStringTokenizer tokenizer(folders, wxT(", "));
		while ( tokenizer.HasMoreTokens() )
		{
			// Do not appoint the ext here, but force to be a file path
			// otherwise only the directory path is returned by wxFileSystemWatcherEvent
			wxFileName path(tokenizer.GetNextToken(), wxEmptyString, wxEmptyString);
			monitor->AddTree( path , wxFSW_EVENT_CREATE ) ;
		}
	}


	monitor->Start();

	return true ;
}

/**
 * Process the creation of new data file
 */
bool ProcessController::OnExportTerminate(int pid, int status, const wxString & script, const wxString & datafile)
{
	// The export is terminated, may success or not
	// in any case, transfer data to storage
	wxGetApp().Log(wxString::Format("Export Terminate PID=%d, STATUS=%d", pid, status));

	TransferFile(datafile);
	return true ;
}

/**
 * Process the creation of new data file
 */
bool ProcessController::OnNewDataFileFound(const wxString & file)
{
	// deal with new file
	if( ! ExportFile(file) )
	{
		// No export? do transfer
		TransferFile(file);
	}
	return true ;
}

/**
 * Export new data file
 */
bool ProcessController::ExportFile(const wxString & file)
{
	wxString equipment_id = ReadSessionMetaData(wxT("session.equipment.id"));

	// let's have a look if export is supported by this equipment
	bool enabled = wxFileConfig::Get()->ReadBool(
			wxString::Format(wxT("equipment.%s.export.enabled"), equipment_id), 
			true
		) ;
	wxGetApp().Log(wxT("Export"));
	wxGetApp().Log(wxString::Format(" Enabled %d", enabled));

	if(enabled)
	{
		// export required, do export before transfer data to storage
		wxString script = wxFileConfig::Get()->Read(
				wxString::Format(wxT("equipment.%s.export.script"), equipment_id)
			) ;
		
		wxGetApp().Log(wxString::Format(" Script %s", script));
		wxGetApp().Log(wxString::Format(" Datafile %s", file));

		MacroScheduler::Get()->Execute(script, file);
		return true ;
	}

	// seems the equipment want data file go storage straight away
	// all right, we can do that by giving a false here
	return false;
}


/**
 * Export new data file
 */
bool ProcessController::TransferFile(const wxString & file)
{
	wxString equipment_id = ReadSessionMetaData(wxT("session.equipment.id"));

	// let's have a look if storage is supported by this equipment
	bool enabled = wxFileConfig::Get()->ReadBool(
			wxString::Format(wxT("equipment.%s.storage.enabled"), equipment_id), 
			true
		) ;

	bool prompt = wxFileConfig::Get()->ReadBool(
			wxString::Format(wxT("equipment.%s.storage.prompt"), equipment_id), 
			true
		) ;
		
	if( enabled )
	{
		if(prompt)
		{
			// Display a promot dialog, start transfer within 30 seconds
		}
		
		// TODO: Transfer file here
		return true ;
	}

	return false;
}