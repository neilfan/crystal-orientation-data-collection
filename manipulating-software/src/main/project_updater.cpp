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
#include <wx/base64.h>
#include <wx/buffer.h>

#include "main/project_updater.h"
#include "main/update_process.h"
#include "main/app.h"

ProjectUpdater * ProjectUpdater::m_pInstance = NULL;


ProjectUpdater::ProjectUpdater()
{
	m_isBusy = false;
	m_timer = new wxTimer(this) ;

	// a temp array of string listing all project ids to be updated
	// will be clearen after all updated
	m_projectIds = new wxArrayString ;
	m_cmd = wxEmptyString ;
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

bool ProjectUpdater::IsBusy()
{
	return m_isBusy;
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
		wxGetApp().Log(wxT("Retrieving project list, please wait"));
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
			wxString password_enc = wxFileConfig::Get()->Read( wxT("sys.network.smb.password") );
			wxString domain  = wxFileConfig::Get()->Read( wxT("sys.network.smb.domain") );

			wxMemoryBuffer mb( wxBase64Decode(password_enc) ) ;
			wxString password((const char *)mb.GetData(), mb.GetDataLen());

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
					// pn here is all the project ids existing on remote server
					m_projectIds->Add( pn ) ;
				}
			}
		}

		wxGetApp().Log(
			wxString::Format("%d project profiles found", m_projectIds->GetCount())
		);

		// got a list of existing pids here
		// store in a list file for further use
		if(m_projectIds->GetCount() > 0)
		{
			wxFileName f(PROJECT_LIST_FILENAME) ;
			wxTextFile f_pl(f.GetFullPath()) ;

			if( ! f.DirExists())
			{
				f.Mkdir();
			}

			wxGetApp().Log( "Generating Project Cache List File" );

			if(f_pl.Exists())
			{
				f_pl.Open();
			}
			else
			{
				f_pl.Create();
			}

			f_pl.Clear();

			// TODO cache this list for other use.
			for(i=0; i<m_projectIds->GetCount(); i++)
			{
				f_pl.AddLine(m_projectIds->Item(i));
			}
			f_pl.Write();
			f_pl.Close();
		}


	}
	else
	{
		// remove current project id from the project-to-update list
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
		wxGetApp().Log("Analysis Project Profile for " + projectId);

		wxString remoteDir = wxFileConfig::Get()->Read(wxT("sys.network.smb.storage")) ;
		remoteDir.Replace(wxT("%PID%"), projectId) ;
		
		// remote file name
		wxFileName rf(remoteDir, PROJECT_FILENAME) ;

		// local file name
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



/**
 * Reset 
 */
void ProjectUpdater :: Reset()
{
	if (m_pInstance != NULL)
	{
		wxDELETE(m_pInstance);
	}
}
