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


#include "main/process_controller.h"
#include "main/datafile_storage.h"
#include "main/async_process.h"
#include "main/app.h"

/**
 * class TransferProcess
 *
 */

// This is the handler for process termination events
//class TransferProcess : public wxProcess

class TransferProcess : public AsyncProcess
{
protected:
	wxFileName      m_file;
	wxFileName      m_dest;
	wxString        m_sessionId ;
	wxString        m_remoteDir ;
	wxArrayString   m_cmds ;
	unsigned long   m_posCmds ;

public:
	TransferProcess(const wxString & sessionId, const wxString & file, const wxString & dest) : AsyncProcess(false, true)
	{
		// load remote project directory
		wxFileName session_ini("sessions", sessionId, "ini");
		wxFileInputStream session( session_ini.GetFullPath() );
		wxFileConfig session_config( session );

		wxString project_id = session_config.Read(wxT("metadata/session.project.id"), wxEmptyString);
		if( project_id!=wxEmptyString)
		{
			m_remoteDir = wxFileConfig::Get()->Read(
					wxT("project.") + project_id + wxT(".storage.defaultpath")
				) ;
		}



		m_sessionId   = sessionId ;
		m_posCmds=0;

		wxFileName from(file) ;
		wxFileName to(dest);

		if( ! to.HasName() )
		{
			to.SetName( from.GetName() ) ;
		}

		if( ! to.HasExt() && from.HasExt() )
		{
			to.SetExt(  from.GetExt()  ) ;
		}

		m_file = from ;
		m_dest = to ;

		if( ! m_dest.HasName() )
		{
			m_dest.SetName( m_file.GetName() ) ;
		}
		
		if( ! m_dest.HasExt() && m_file.HasExt() )
		{
			m_dest.SetExt(  m_file.GetExt()  ) ;
		}
		
		m_dest.InsertDir(0, m_sessionId) ;
		wxArrayString dirs = m_dest.GetDirs() ;
		wxString currentDir = wxEmptyString ;

		size_t i ;
		for(i=0; i<dirs.GetCount(); i++)
		{
			currentDir += dirs[i] + wxFileName::GetPathSeparator(wxPATH_UNIX ) ;
			m_cmds.Add( wxString::Format("mkdir \\\"%s\\\"", currentDir) );
		}
		m_cmds.Add(wxString::Format("put \\\"%s\\\" \\\"%s\\\"", m_file.GetFullPath(), m_dest.GetFullPath()));
	}

	const wxFileName & GetFile()
	{
		return m_file ;
	}

	const wxFileName & GetDest()
	{
		return m_dest ;
	}

	const wxString GetFirstCommand()
	{
		m_posCmds=0;
		if(m_cmds.IsEmpty())
		{
			return wxEmptyString ;
		}
		else
		{
			return m_cmds[m_posCmds] ;
		}
	}

	const wxString GetNextCommand()
	{
		m_posCmds++ ;
		if(!m_cmds.IsEmpty() && m_posCmds < m_cmds.GetCount() )
		{
			return m_cmds[m_posCmds] ;
		}

		return wxEmptyString ;
	}
	
	const wxString & GetRemoteDir()
	{
		return m_remoteDir ;
	}

	const wxString & GetSessionId()
	{
		return m_sessionId;
	}
	
	// instead of overriding this virtual function we might as well process the
	// event from it in the frame class - this might be more convenient in some
	// cases
	virtual void OnTerminate(int pid, int status)
	{
		DataFileStorage::Get()->OnTransferTerminate(status, this) ;
	}

};


DataFileStorage * DataFileStorage::m_pInstance = NULL;


DataFileStorage::DataFileStorage()
{
	m_timer = new wxTimer(this) ;
	Connect( wxEVT_TIMER , wxTimerEventHandler( DataFileStorage::OnTimer ));

	m_isTransferring = false ;
	
	m_timer_interver = wxFileConfig::Get()->ReadLong (
			wxT("sys.network.checking.interval"),
			DATAFILE_STORAGE_TIMER_INTERVAL
		) ;
}


DataFileStorage::~DataFileStorage()
{
	Disconnect( wxEVT_TIMER , wxTimerEventHandler( DataFileStorage::OnTimer ));
	wxDELETE(m_timer);
}

/**
 * Return the instance
 * Singleton pattern.
 */
DataFileStorage * DataFileStorage::Get()
{
	if( ! m_pInstance)
	{
		m_pInstance = new DataFileStorage;
	}
	return m_pInstance ;
}

bool DataFileStorage::Start()
{
	if( ! IsRunning() )
	{
		// To start transfer, just start the timer and everything will go through
		return m_timer->Start(m_timer_interver);
	}
	return true ;
}

bool DataFileStorage::Stop()
{
	if( IsRunning() )
	{
		// to stop the transfer
		// fist we need to make sure the timer is stopped
		m_timer->Stop();

		m_isTransferring = false ;
	}
	return true ;
}

void DataFileStorage::OnTimer( wxTimerEvent& event )
{
	
	if( m_sessionTasks.IsEmpty() )
	{

		// check if any task exists
		wxFileName cache(DATAFILE_STORAGE_CACHE_FILENAME) ;

		wxFileInputStream input( cache.GetFullPath() );
		if( ! input.IsOk() )
		{
			// failed to load cache file
			// wait for next pulse
			return ;
		}

		wxTextInputStream text( input );

		while( ! text.GetInputStream().Eof() )
		{
			wxString line = text.ReadLine() ;
			
			// the line in CACHE file is expected to be a session_id
			// in the the ./sessions folder
			
			// for security, this directory name should not contain any \ or /
			// You do not want all your disk files transferred to network storage, right?

			if(
				line==wxEmptyString ||
				line.Find( wxUniChar('/') ) != wxNOT_FOUND ||
				line.Find( wxUniChar('\\') ) != wxNOT_FOUND )
			{
				// it's not a good session id
				continue ;
			}
			
			// found a new session to be transferred

			wxFileName session_ini("sessions", line, "ini");
			wxGetApp().Log(wxString::Format("Scanning session file: %s", session_ini.GetFullPath()));
			if( ! session_ini.FileExists() )
			{
				// bad session id, ini file not exist
				continue ;
			}

			// TODO:load file list from exchange file
			// and put in a queue
			// the format of exchange file is available on
			// http://code.google.com/p/crystal-orientation-data-collection/wiki/SoftwareInterfaceStandard

			// Open the config file
			wxFileInputStream session( session_ini.GetFullPath() );
			wxFileConfig session_config( session );

			// Get the counts of files
			long count = session_config.ReadLong(wxT("files/count"), 0);
			wxGetApp().Log(wxString::Format("Session file claims %d files to be transferred", count));

			if( count == 0)
			{
				// no task for this session
				continue ;
			}


			
			m_currentTransferSessionID = line ;
			m_sessionTasks.Add( session_ini.GetFullPath() + DATAFILE_STORAGE_LINE_DELIM + wxT("METADATA"));
			wxGetApp().Log(wxString::Format("Adding session data file '%s' to be transferred to 'METADATA'", session_ini.GetFullPath()));

			long i ;
			for(i=1; i<=count ;i++)
			{
				wxString key_file = wxString::Format("files/file%d", i) ;
				wxString filex = wxEmptyString ;
				wxString destx = wxEmptyString ;

				if(session_config.HasEntry(key_file))
				{
					filex = session_config.Read(key_file, wxEmptyString) ;
					if( filex!=wxEmptyString && wxFileExists( filex ) )
					{
						destx = session_config.Read(wxString::Format("files/destination%d", i) , wxEmptyString);
						m_sessionTasks.Add( filex + DATAFILE_STORAGE_LINE_DELIM + destx );
						wxGetApp().Log(wxString::Format("Adding session data file '%s' to be transferred to '%s'", filex, destx));
					}
				}
			}

			wxGetApp().Log(wxString::Format("Transferring session ID %s", m_currentTransferSessionID));



			// stop loop, we will not look for any further sessions.
			// when all transfer completed, this session will be removed from CACHE
			break ;

		}
	}

	// Here, we have a list of task to go
	if( ! m_sessionTasks.IsEmpty() && ! m_isTransferring)
	{
		m_isTransferring = true ;
		wxArrayString array = wxStringTokenize(m_sessionTasks[0],DATAFILE_STORAGE_LINE_DELIM) ;
		Transfer(array[0], array[1]) ;
	}
}

const wxString & DataFileStorage::GetCommandLine()
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
	return m_cmd ;

}

bool DataFileStorage::Transfer(const wxString & datafile, const wxString & dest)
{

	TransferProcess * process = new TransferProcess(m_currentTransferSessionID, datafile, dest);


	wxString cmd = process->GetFirstCommand() ;
	if( cmd != wxEmptyString )
	{
		wxString cmd_line = wxString::Format("%s -D \"%s\" -c \"%s\"",
					GetCommandLine(),
					process->GetRemoteDir(),
					cmd );
		wxGetApp().Log(wxString::Format("  Executing remote command %s", cmd));
		long pid = wxExecute(cmd_line, wxEXEC_ASYNC | wxEXEC_MAKE_GROUP_LEADER | wxEXEC_HIDE_CONSOLE , process);
		wxGetApp().Log(wxString::Format("  PID %d", pid));
	}
	else
	{
		// this file in session is done
		// continue with next file
	}
	return true ;
}


bool DataFileStorage::OnTransferTerminate(int status, TransferProcess * process)
{
	wxFileName file     = process->GetFile() ;
	wxFileName dest     = process->GetDest() ;

	wxGetApp().Log(wxString::Format("Transfer %d Completed %s -> %s", process->GetPid(), file.GetFullPath(), dest.GetFullPath()));
	wxGetApp().Log(wxString::Format("Transfer %d Status %d", process->GetPid(), status));

	if(status==0)
	{
		wxString cmd = process->GetNextCommand() ;
		if( cmd != wxEmptyString )
		{
			wxString cmd_line = wxString::Format("%s -D \"%s\" -c \"%s\"",
						GetCommandLine(),
						process->GetRemoteDir(),
						cmd );
			wxGetApp().Log(wxString::Format("  Executing remote command %s", cmd));
			long pid = wxExecute(cmd_line, wxEXEC_ASYNC | wxEXEC_MAKE_GROUP_LEADER | wxEXEC_HIDE_CONSOLE , process);
			wxGetApp().Log(wxString::Format("  PID %d", pid));
		}
		else
		{
			// this file in session is done
			// Here, we have a list of task to go
			if( ! m_sessionTasks.IsEmpty())
			{
				m_sessionTasks.RemoveAt(0);
			}

			if( ! m_sessionTasks.IsEmpty() && m_sessionTasks[0]!=wxEmptyString)
			{
				wxArrayString array = wxStringTokenize(m_sessionTasks[0],DATAFILE_STORAGE_LINE_DELIM) ;
				if(array.GetCount()==1)
				{
					// by default upload to SESSION directory
					array.Add(wxT("./"));
				}
				Transfer(array[0], array[1]) ;
			}
			else
			{
				// session is done
				FinaliseSession( process->GetSessionId() );
			}
		}
		return true ;
	}

	// if status is not 0
	// an error occured, stop transfer and wait for next try
	m_isTransferring = false;

	return false;
}

bool DataFileStorage::AddTask(const wxFileName & from, const wxFileName & to)
{
		/*
			TODO: these lines are for backup only

			wxString project_id = ProcessController::Get()->ReadSessionMetaData(wxT("session.project.id"));
			wxString project_directory = wxFileConfig::Get()->Read(
					wxT("project.") + project_id + wxT(".storage.defaultpath")
				) ;

			if(project_directory == wxEmptyString)
			{
				wxGetApp().Log(
					wxString::Format("Invalid Remote storage path for project %s", project_id)
				);

				return false;
			}
		*/
	return true ;
}

void DataFileStorage::FinaliseSession(const wxString & sessionID)
{
	wxFileName filename(DATAFILE_STORAGE_CACHE_FILENAME) ;
	wxTextFile cache ;
	cache.Open(filename.GetFullPath()) ;
	size_t i ;
	for(i=0; i<cache.GetLineCount() ; i++)
	{
		if( cache.GetLine(i) == sessionID )
		{
			cache.RemoveLine(i);
			break;
		}
	}
	cache.Write();
	cache.Close();
	m_isTransferring = false;
}
