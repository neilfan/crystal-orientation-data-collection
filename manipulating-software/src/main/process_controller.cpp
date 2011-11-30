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
#include <wx/filename.h> 
#include <wx/wfstream.h> 
#include <wx/msgdlg.h> 
#include <wx/fileconf.h> 

#include "main/process_controller.h"
#include "main/confirm_dialog.h"
#include "main/app.h"

ProcessController * ProcessController::m_pInstance = NULL;

ProcessController::ProcessController()
{
	m_confirm_dialog = NULL ;
}

ProcessController::~ProcessController()
{
	if(m_confirm_dialog!=NULL)
	{
		wxDynamicCast(m_confirm_dialog, ConfirmDialog)->Destroy();
	}
}

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

void ProcessController::StartNewSession(const wxString & exchange_file)
{
	wxGetApp().Log(_T("Starting session with exchange file ") + exchange_file);

	// TODO: STOP CURRENT SESSION FIRST

	/**
	 * Create Session directory structure
	 */
	wxString session_id ;
	wxFileName session_filename ;

	do
	{
		session_id = wxString::Format(wxT("%u-%04u"), wxDateTime::GetTimeNow() , rand()%10000);
		session_filename.AssignDir(wxT("sessions")) ;
		session_filename.AppendDir(session_id);
	} while(session_filename.DirExists()) ;

	// session has been assigned an unique id
	if( ! session_filename.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL))
	{
		wxGetApp().Log(wxT("Failed to create dir ") + session_filename.GetPath());
	}
	else
	{
		m_current_session_id = session_id ;

		wxGetApp().Log(wxT("Created session folder ") + session_filename.GetPath());
		session_filename.SetName(SESSION_FILENAME_METADATA) ;
		session_filename.SetEmptyExt();
		wxCopyFile(exchange_file, session_filename.GetFullPath());
	}
	// END Create Session Ddirectory structure 
	
	LaunchEquipment();
	
	// TODO: Start Monitor files here
}


bool ProcessController::LaunchEquipment()
{
	wxString equipment_id = ReadSessionMetaData(_T("session.equipment.id"));


	long launch_enabled = wxFileConfig::Get()->ReadLong (
			wxT("equipment.")
			+ equipment_id
			+ wxT(".launcher.enabled"),
			0
		) ;

	if( false && launch_enabled != 1 )
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
