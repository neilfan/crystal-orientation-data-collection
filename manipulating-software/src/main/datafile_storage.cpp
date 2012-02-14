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

#include "main/process_controller.h"
#include "main/datafile_storage.h"
#include "main/async_process.h"
#include "main/transfer_process.h"
#include "main/app.h"


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

	// remove the tmp cache file
	wxFileName cache(DATAFILE_STORAGE_CACHE_FILENAME) ;
	wxString cache_tmp (cache.GetFullPath() + wxT(".TMP") );
	if( wxFileExists(cache_tmp))
	{
		wxRemoveFile(cache_tmp);
	}

	return true ;
}

void DataFileStorage::OnTimer( wxTimerEvent& event )
{
	
	if( m_sessionTasks.IsEmpty() )
	{
		wxFileName cache(DATAFILE_STORAGE_CACHE_FILENAME) ;

		if( ! cache.FileExists())
		{
			return ;
		}

		// check if any task exists
		wxString cache_tmp (cache.GetFullPath() + wxT(".TMP") );
		wxFileInputStream input( cache_tmp );
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
			if( ! session_ini.FileExists() )
			{
				// bad session id, ini file not exist
				FinaliseSession(line);
				continue ;
			}

			// TODO:load file list from exchange file
			// and put in a queue
			// the format of exchange file is available on
			// http://code.google.com/p/crystal-orientation-data-collection/wiki/SoftwareInterfaceStandard

			// Open the config file
			wxFileConfig session_config( 
				wxEmptyString,
				wxEmptyString, 
				session_ini.GetFullPath(),
				wxEmptyString,
				wxCONFIG_USE_LOCAL_FILE|wxCONFIG_USE_NO_ESCAPE_CHARACTERS
			);

			// Get the counts of files
			long count = session_config.ReadLong(wxT("files/count"), 0);

			if( count == 0)
			{
				// no task for this session
				FinaliseSession(line);
				continue ;
			}

			wxGetApp().Log(wxString::Format("Scanning session file: %s", session_ini.GetFullPath()));
			wxGetApp().Log(wxString::Format("Session file claims %d files to be transferred", count));

			m_currentTransferSessionID = line ;
			m_sessionTasks.Add( session_ini.GetFullPath() + DATAFILE_STORAGE_LINE_DELIM + DATAFILE_STORAGE_METADATA_FILENAME);
			wxGetApp().Log(wxString::Format("Adding session data file '%s' to be transferred to '%s'", session_ini.GetFullPath(), DATAFILE_STORAGE_METADATA_FILENAME));

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
		if(array.GetCount()==1)
		{
			// by default upload to SESSION directory
			array.Add(wxT("./"));
		}
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
		wxExecute(cmd_line, wxEXEC_ASYNC | wxEXEC_MAKE_GROUP_LEADER | wxEXEC_HIDE_CONSOLE , process);
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


	if(status==0)
	{
		wxString cmd = process->GetNextCommand() ;
		if( cmd != wxEmptyString )
		{
			wxString cmd_line = wxString::Format("%s -D \"%s\" -c \"%s\"",
						GetCommandLine(),
						process->GetRemoteDir(),
						cmd );
			 wxExecute(cmd_line, wxEXEC_ASYNC | wxEXEC_MAKE_GROUP_LEADER | wxEXEC_HIDE_CONSOLE , process);
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
	else
	{
		wxGetApp().Log(wxString::Format("Transfer %d Failed %s -> %s", process->GetPid(), file.GetFullPath(), dest.GetFullPath()));
		wxGetApp().Log(wxString::Format("Transfer %d Status %d", process->GetPid(), status));
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
	wxTextFile cache(filename.GetFullPath()) ;
	cache.Exists() ? cache.Open() : cache.Create() ;

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
