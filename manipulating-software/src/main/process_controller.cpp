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
#include "main/launchequipment_process.h"
#include "main/convertdata_process.h"
#include "main/export_process.h"
#include "main/datafile_storage.h"
#include "main/macro_scheduler.h"
#include "main/dialog.h"
#include "main/confirm_dialog.h"
#include "main/notify_dialog.h"
#include "main/async_process.h"
#include "main/app.h"

ProcessController * ProcessController::m_pInstance = NULL;

ProcessController::ProcessController()
{
	m_confirm_dialog = NULL ;
	m_current_session_id = wxEmptyString ;
	
	// start the file transfer daemon
	DataFileStorage::Get()->Start() ;
}

ProcessController::~ProcessController()
{
	FinaliseSession();
	DataFileStorage::Get()->Stop() ;

	if(m_confirm_dialog!=NULL)
	{
		wxDynamicCast(m_confirm_dialog, ConfirmDialog)->Destroy();
	}
	
	NotifyDialog::Get()->Destroy();
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

wxFileName ProcessController::GetCurrentSessionFileName()
{
	wxFileName cfg_filename ("sessions", m_current_session_id, "ini") ;
	return cfg_filename ;
}

/**
 * Get the setting for current session from the METADATA file
 */
wxString ProcessController::GetMetadata(const wxString & key, const wxString & defaultVal)
{
	if( m_current_session_id == wxEmptyString )
	{
		return defaultVal ;
	}
	
	wxFileName cfg_filename = GetCurrentSessionFileName() ;

	if(cfg_filename.FileExists())
	{
		wxFileInputStream fis(cfg_filename.GetFullPath()) ;
		wxFileConfig metadata_config(fis);
		metadata_config.SetStyle( metadata_config.GetStyle() | wxCONFIG_USE_NO_ESCAPE_CHARACTERS ) ;
		return metadata_config.Read(wxT("metadata/") + key, defaultVal);

	}
	return defaultVal ;
}

wxString ProcessController::GetEquipmentId()
{
	return GetMetadata(wxT("session.equipment.id")) ;
}

wxString ProcessController::GetProjectId()
{
	return GetMetadata(wxT("session.project.id")) ;
}


/**
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


/**
 * Start a new session
 * @exchange_file A temp file in same format of Session INI
 *
 * Copy the exchange_file to /sessions/ directory and rename
 */
void ProcessController::StartNewSession(const wxString & exchange_file)
{
	wxGetApp().Log(wxT("Starting session with exchange file ") + exchange_file);

	// STOP PREVIOUS SESSION FIRST
	FinaliseSession() ;

	wxFileInputStream cfg_stream(exchange_file);

	if(wxFileExists(exchange_file))
	{
		wxFileInputStream fis(exchange_file) ;
		wxFileConfig metadata_config(fis);
		metadata_config.SetStyle( metadata_config.GetStyle() | wxCONFIG_USE_NO_ESCAPE_CHARACTERS ) ;

		/**
		 * Create Session INI
		 * name format session_id.ini where session_id is a time-based string
		 */
		wxString session_id =  wxDateTime::Now().Format(DATETIME_FORMAT_DEFAULT);
		wxFileName session_filename("sessions", session_id , "ini") ;

		// session has been assigned an unique id
		if( !session_filename.DirExists() )
		{
			wxGetApp().Log(wxString::Format(wxT("Creating dir %s") , session_filename.GetPath()));
			session_filename.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
		}

		m_current_session_id = session_id ;

		wxCopyFile(exchange_file, session_filename.GetFullPath());

		// END Create Session Ddirectory structure 
		if( metadata_config.Read(wxT("metadata/session.project.id")) != wxEmptyString )
		{
			StartMonitoring();
		}
	}

	NotifyDialog::Get()->Show(false);
	MainDialog::Get()->Show(false);


	LaunchEquipment();
}


/**
 * Launch the equuipment
 * @return executed. Ture if launch required, otherwise false
 */
bool ProcessController::LaunchEquipment()
{
	wxString equipment_id = GetEquipmentId();

	bool launch_enabled = wxFileConfig::Get()->ReadBool (
			wxString::Format(wxT("equipment.%s.launcher.enabled"), equipment_id),
			false
		) ;

	wxString launch_program = wxFileConfig::Get()->Read(
			wxString::Format(wxT("equipment.%s.launcher.program"), equipment_id)
		) ;

	if( launch_enabled && launch_program!=wxEmptyString)
	{
		wxGetApp().Log(wxT("Starting program ") + launch_program);
	}
	else
	{
		launch_program = DUMMY_PROGRAM_CMD ;
	}

	LaunchEquipmentProcess * process = new LaunchEquipmentProcess(m_current_session_id) ;

	wxFileName launch_file(launch_program);
	long pid = wxExecute (launch_file.GetFullPath(), wxEXEC_ASYNC, process);
	wxGetApp().Log(wxString::Format("LaunchEquipment Program PID %d", pid));

	return pid != 0 ;
		

	return false;
}

/**
 * The equipment software exits
 * Need to export text-data/convert format/transfer to ISILON
 */
bool ProcessController::OnLaunchEquipmentTerminate(int pid, int status, LaunchEquipmentProcess * process)
{
	// finalise current session when equipment exits
	if( m_current_session_id == process->GetSessionId() &&
		m_current_session_id != wxEmptyString &&
		GetProjectId() != wxEmptyString
	)
	{
		// export the file
		Export() ;
	}
	return true ;
}


bool ProcessController::StartMonitoring()
{
	// TODO: Start Monitor files here
	DataFileMonitor::Reset();
	DataFileMonitor * monitor = DataFileMonitor::Get() ;
	
	wxString equipment_id = GetEquipmentId();


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

		wxStringTokenizer tokenizer(folders, wxT(", ;"));
		while ( tokenizer.HasMoreTokens() )
		{
			// Do not appoint the ext here, but force to be a file path
			// otherwise only the directory path is returned by wxFileSystemWatcherEvent
			wxFileName path(tokenizer.GetNextToken(), wxEmptyString, wxEmptyString);
			if(path.DirExists())
			{
				monitor->AddTree( path , wxFSW_EVENT_CREATE ) ;
			}
		}
	}


	monitor->Start();

	return true ;
}

/**
 * Process the creation of new data file
 */
bool ProcessController::OnNewDataFileFound(const wxString & file)
{
	// shall we ignore all sub-folders?
	bool isFlatFolder = wxFileConfig::Get()->ReadBool(
			wxString::Format(wxT("equipment.%s.storage.flatfolder"), GetEquipmentId()),
			false
		) ;

	wxFileName filename(file);
	wxFileName relativePath(file);

	DataFileMonitor * monitor = DataFileMonitor::Get() ;
	wxArrayString * paths = new wxArrayString;
	size_t i ;

	monitor->GetWatchedPaths(paths) ;
	
	// calc the relative path
	for( i=0 ;i<paths->GetCount() ; i++)
	{
		wxFileName path( paths->Item(i) ) ;
		if( relativePath.GetFullPath().Find( path.GetFullPath() ) == 0 )
		{
			// make it a relative path
			relativePath.MakeRelativeTo(path.GetFullPath());
			continue;
		}
	}

	// put the file in ini
	wxFileName cfg_filename = GetCurrentSessionFileName() ;

	if(cfg_filename.FileExists())
	{
		wxFileInputStream fis(cfg_filename.GetFullPath()) ;
		wxFileConfig config(fis);
		config.SetStyle( config.GetStyle() | wxCONFIG_USE_NO_ESCAPE_CHARACTERS ) ;

		// enumeration variables
		wxString str;
		
		str = filename.GetFullPath() ;
		str.Replace(wxT("\\"), wxT("/")) ;

		long count = config.ReadLong(wxT("files/count"), 0) ;
		
		// check if the file is already recorded
		// duplication will happen when monitoring a Network Location
		long i ;
		for(i=1; i<=count; i++)
		{
			wxString fn = config.Read(wxString::Format("files/file%d", i)) ;
			if( fn!=wxEmptyString && fn==str)
			{
				return true ;
			}
		}
		
		count ++ ;
		config.Write(wxT("files/count"), count) ;
		config.Write(
			wxString::Format("files/file%d", count),
			str
			);

		// if isFlatFolder is true, ignore all sub-folder settings
		// all files will be stored in a flat directory
		// Warning: files with same name may be overwritten
		str = relativePath.GetPath(wxPATH_GET_SEPARATOR) ;
		str.Replace(wxT("\\"), wxT("/")) ;
		config.Write(
			wxString::Format("files/destination%d", count),
			(relativePath.IsRelative() && !isFlatFolder) ? str : wxT("")
		);

		wxFileOutputStream out_stream(cfg_filename.GetFullPath());
		if(out_stream.IsOk())
		{
			config.Save(out_stream);
			out_stream.Close();
		}
	}

	return true ;
}

bool ProcessController::IsExportEnabled()
{
	// let's have a look if export is supported by this equipment
	return wxFileConfig::Get()->ReadBool(
			wxString::Format(wxT("equipment.%s.export.enabled"), GetEquipmentId()),
			false
		) ;
}

/**
 * Export new data file
 */
void ProcessController::Export()
{
	wxString cmd = wxEmptyString;

	if(IsExportEnabled())
	{
		// export required, do export before transfer data to storage
		wxString program = wxFileConfig::Get()->Read(
				wxString::Format(wxT("equipment.%s.export.program"), GetEquipmentId())
			) ;

		cmd = wxString::Format(
						wxT("%s \"%s\""),
						program,
						GetCurrentSessionFileName().GetFullPath()
						);

		wxGetApp().Log(wxString::Format("Export cmd %s", cmd));

	}
	
	if( cmd == wxEmptyString)
	{
		cmd = DUMMY_PROGRAM_CMD ;
	}

	MacroScheduler::Get()->Execute(cmd);
}

/**
 * Process the creation of new data file
 */
bool ProcessController::OnExportTerminate(int pid, int status, ExportProcess * process)
{
	// The export is terminated, may success or not
	// in any case, transfer data to storage
	wxGetApp().Log(wxString::Format("Export Terminate PID=%d, STATUS=%d", pid, status));
	
	// IsConvertEnabled() ? Convert() : FinaliseSession() ;
	Convert() ;

	return true ;
}

bool ProcessController::IsConvertEnabled()
{
	return wxFileConfig::Get()->ReadBool (
			wxString::Format("equipment.%s.processing.enabled", GetEquipmentId()),
			false
		) ;
}
/**
 * Convert data files
 */
void ProcessController::Convert()
{
	wxString cmd ;
	wxString convert_program ;
	if( IsConvertEnabled() )
	{
		convert_program = wxFileConfig::Get()->Read(
			wxString::Format("equipment.%s.processing.program", GetEquipmentId())
			) ;
		wxGetApp().Log(wxT("Starting processing program ") + convert_program);
	}

	ConvertDataProcess * process = new ConvertDataProcess(m_current_session_id) ;

	if( convert_program != wxEmptyString )
	{
		wxFileName filename(convert_program);
		cmd = wxString::Format(
				"%s --launched-from-manipulating-software --research-exchange-file \"%s\"",
				filename.GetFullPath(),
				GetCurrentSessionFileName().GetFullPath()
			);
	}
	else
	{
		cmd = DUMMY_PROGRAM_CMD ;
	}

	long pid = wxExecute(cmd, wxEXEC_ASYNC, process);
	wxGetApp().Log(wxString::Format("Processing Program PID %d", pid));
}

bool ProcessController::OnConvertTerminate(int pid, int status, ConvertDataProcess * process)
{
	// check the error code
	wxString error = GetMetadata(wxT("error.code"), wxT("0"));

	// finalise current session when equipment exits
	if( wxAtoi(error) == 0 &&
		m_current_session_id == process->GetSessionId() &&
		m_current_session_id != wxEmptyString &&
		GetProjectId() != wxEmptyString
	)
	{
		FinaliseSession();
	}
	return true ;
}

/**
 * Finalise current session
 * What will happen:
 *   1. Write current session ID to CACHE file
 *      (so that all files will be synced to storage)
 *   2. Reset Monitoring
 * 
 * @return true when success, otherwise false
 */
void ProcessController::FinaliseSession()
{
	// ITEM 1 : add session to CACHE for storage
	if(
		m_current_session_id != wxEmptyString &&
		GetProjectId() != wxEmptyString
	)
	{
		wxGetApp().Log(
			wxString::Format(wxT("Session %s Finalised.") , m_current_session_id )
		);

		wxFileName csfn = GetCurrentSessionFileName() ;
		if(csfn.FileExists())
		{

			NotifyDialog::Get()->Notify(csfn.GetFullPath());

			// if no current session, no action required
			wxString str ;
			bool is_id_in_cache = false;

			wxFileName filename(DATAFILE_STORAGE_CACHE_FILENAME) ;
			wxTextFile cache(filename.GetFullPath()) ;

			cache.Exists() ? cache.Open() : cache.Create() ;

			for ( str = cache.GetFirstLine(); !cache.Eof(); str = cache.GetNextLine() )
			{
				if(str == m_current_session_id)
				{
					is_id_in_cache = true ;
					break;
				}
			}
			
			if( ! is_id_in_cache)
			{
				cache.AddLine(m_current_session_id);
			}
			cache.Write();
			cache.Close();
		}
		
		m_current_session_id = wxEmptyString ;
	}

	// ITEM 2: stop monitoring
	DataFileMonitor * monitor = DataFileMonitor::Get() ;
	monitor->Reset();
	


}