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
#include <wx/process.h> 
#include <wx/regex.h> 
#include <wx/wfstream.h> 
#include <wx/txtstrm.h> 
#include <wx/fileconf.h> 
#include <wx/filename.h> 
#include <wx/arrstr.h> 
#include <wx/stream.h> 
#include <wx/string.h> 
#include <wx/tokenzr.h>


#include "main/project_updater.h"
#include "main/update_process.h"
#include "main/app.h"

ProjectUpdater * ProjectUpdater::m_pInstance = NULL;


ProjectUpdater::ProjectUpdater()
{
	m_isBusy = false;
	m_timer = new wxTimer(this) ;
	m_projectIds = new wxArrayString ;
	Connect( wxEVT_TIMER , wxTimerEventHandler( ProjectUpdater::OnTimer ));
}


ProjectUpdater::~ProjectUpdater()
{
	Disconnect( wxEVT_TIMER , wxTimerEventHandler( ProjectUpdater::OnTimer ));
	wxDELETE(m_timer);
}

/**
 * Return the instance
 * Singleton pattern.
 */
ProjectUpdater * ProjectUpdater::Get()
{
	if( ! m_pInstance)
	{
		m_pInstance = new ProjectUpdater;
	}
	return m_pInstance ;
}

bool ProjectUpdater::Start()
{
	if( ! m_timer->IsRunning() )
	{
		return m_timer->Start(INTERVAL_DAY);
	}
	
	return true ;
}


void ProjectUpdater::OnTimer( wxTimerEvent& event )
{
	Update();
}

void ProjectUpdater::Update()
{
	if( ! m_isBusy )
	{
		m_isBusy = true ;
		UpdateProcess * process = new UpdateProcess(wxEmptyString, true);

		wxString cmd_line = GetCommandLine(wxT("dir")) ;

		wxGetApp().Log(wxT("Updating Project Profiles"));
		wxExecute(cmd_line, wxEXEC_ASYNC | wxEXEC_MAKE_GROUP_LEADER | wxEXEC_HIDE_CONSOLE , process);
	}
}

wxString ProjectUpdater::GetCommandLine(const wxString & cmd)
{
	if(m_cmd==wxEmptyString)
	{
		wxString service = wxFileConfig::Get()->Read( wxT("sys.network.smb.service") );
		wxString smbclient = wxFileConfig::Get()->Read( wxT("sys.utils.smbclient.program") );

		if(service != wxEmptyString)
		{
			wxString username = wxFileConfig::Get()->Read( wxT("sys.network.smb.username") );
			wxString password = wxFileConfig::Get()->Read( wxT("sys.network.smb.password") );
			wxString domain  = wxFileConfig::Get()->Read( wxT("sys.network.smb.domain") );

			m_cmd = wxString::Format(wxT("%s \"%s\" ") , smbclient, service);

			if(password != wxEmptyString)
			{
				m_cmd += wxString::Format(wxT(" \"%s\"") , password) ;
			}
			else
			{
				// if no password configed, avoid prompt in shell
				m_cmd += wxT(" -N")  ;
			}

			if(username != wxEmptyString)
			{
				m_cmd += wxString::Format(wxT(" -U \"%s\"") , username) ;
			}

			if(domain != wxEmptyString)
			{
				m_cmd += wxString::Format(wxT(" -W \"%s\"") , domain) ;
				m_cmd += wxString::Format(wxT(" -n \"%s\"") , domain) ;
			}


		}
	}
	return wxString::Format("%s -c \"%s\"",
					m_cmd,
					cmd ) ;

}



bool ProjectUpdater::OnCommandTerminate(int status, UpdateProcess * process)
{

	if( process->GetProjectId() == wxEmptyString )
	{
		// retrieving list of folders
		wxRegEx reFolder ( " *(.*) +D +0 +.*" );
		wxArrayString output = process->GetStdOutput() ;
		size_t i ;
		for(i=0; i<output.GetCount(); i++)
		{
			if( reFolder.Matches( output.Item(i) ) )
			{
				wxString pn = reFolder.GetMatch(output.Item(i),1).Trim(true).Trim(false) ;
				if( pn != wxT(".") && pn != wxT("..") )
				{
					m_projectIds->Add( pn ) ;
				}
			}
		}
	}
	else
	{
		m_projectIds->Remove( process->GetProjectId() );
	}

	
	if(m_projectIds->IsEmpty())
	{
		m_isBusy = false ;
		wxGetApp().Log(wxT("Project Profiles Updated"));

	}
	else
	{
		wxString projectId = m_projectIds->Item(0) ;

		wxString remoteDir = wxFileConfig::Get()->Read(wxT("sys.network.smb.storage")) ;
		remoteDir.Replace(wxT("%PID%"), projectId) ;
		
		wxFileName rf(remoteDir, PROJECT_FILENAME) ;
		wxFileName lf(wxT("projects/"), projectId , wxT("ini"));
		if( ! lf.DirExists())
		{
			lf.Mkdir();
		}
		
		wxString cmd = wxString::Format("get %s %s", rf.GetFullPath(), lf.GetFullPath());

		UpdateProcess * process = new UpdateProcess(projectId, false);

		wxString cmd_line = GetCommandLine(cmd) ;
		wxExecute(cmd_line, wxEXEC_ASYNC | wxEXEC_MAKE_GROUP_LEADER | wxEXEC_HIDE_CONSOLE , process);

	}


	return true;
}
