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

#include "main/process_controller.h"
#include "main/datafile_storage.h"
#include "main/app.h"

/**
 * class ValidateProcess
 *
 */

// This is the handler for process termination events
class ValidateProcess : public wxProcess
{
protected:
	wxString m_datafile;
	wxString m_destination;

public:
	ValidateProcess(const wxString& script, const wxString& datafile, const wxString & dest)
		:wxProcess(), m_datafile(datafile), m_destination(dest)
	{
		Redirect();
	}

	// instead of overriding this virtual function we might as well process the
	// event from it in the frame class - this might be more convenient in some
	// cases
	virtual void OnTerminate(int pid, int status)
	{

		DataFileStorage::Get()
			->OnValidateTerminate(pid, status, m_datafile, m_destination) ;
	}

};

/**
 * class TransferProcess
 *
 */

// This is the handler for process termination events
class TransferProcess : public wxProcess
{
protected:
	wxString m_datafile;
	wxString m_destination;

public:
	TransferProcess(const wxString& script, const wxString& datafile, const wxString & dest)
		:wxProcess(), m_datafile(datafile), m_destination(dest)
	{
		Redirect();
	}

	// instead of overriding this virtual function we might as well process the
	// event from it in the frame class - this might be more convenient in some
	// cases
	virtual void OnTerminate(int pid, int status)
	{

		DataFileStorage::Get()
			->OnTransferTerminate(pid, status, m_datafile, m_destination) ;
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
		
		// then we will kill all on-going transferring
		wxKill  ( m_childPid, wxSIGTERM, NULL, wxKILL_CHILDREN );  
		
		m_isTransferring = false ;
	}
	return true ;
}

void DataFileStorage::OnTimer( wxTimerEvent& event )
{
	
	// check if any task exists
	wxFileName cache(DATAFILE_STORAGE_CACHE_FILENAME) ;
	wxRegEx    reg(DATAFILE_STORAGE_CACHE_REGEX) ;
	
	wxFileInputStream input( cache.GetFullPath() );
	if( input.IsOk())
	{
		wxTextInputStream text( input );

		while( ! text.GetInputStream().Eof() )
		{
			wxString line = text.ReadLine() ;

			if ( reg.Matches(line) )
			{
				wxString from = reg.GetMatch(line, 1);
				wxString to   = reg.GetMatch(line, 2);
				
				// the file exist ?
				if ( wxFileExists(from) )
				{
					// copy file
					m_isTransferring = false ;
					m_timer->Stop();
					wxGetApp().Log(
						wxString::Format("copy %s to %s", from, to)
					);
					
					Transfer( from, to);

				}
			}
		}
	}

}


bool DataFileStorage::Transfer(const wxString & datafile, const wxString & dest)
{
	wxString service = wxFileConfig::Get()->Read( wxT("sys.network.smb.service") );
	wxFileName from(datafile);
	wxFileName to(dest);

	if(service == wxEmptyString)
	{
		return false;
	}

	wxString username = wxFileConfig::Get()->Read( wxT("sys.network.smb.username") );
	wxString password = wxFileConfig::Get()->Read( wxT("sys.network.smb.password") );
	wxString domain  = wxFileConfig::Get()->Read( wxT("sys.network.smb.domain") );
	wxString project_directory = to.GetPath(wxPATH_GET_SEPARATOR, wxPATH_UNIX );
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

	wxString cmd_line = wxString::Format(wxT("utils/samba-tng/smbclient.exe \"%s\" ") , service);

	if(password != wxEmptyString)
	{
		cmd_line += wxString::Format(wxT(" \"%s\"") , password) ;
	}
	else
	{
		// if no password configed, avoid prompt in shell
		cmd_line += wxT(" -N")  ;
	}

	if(username != wxEmptyString)
	{
		cmd_line += wxString::Format(wxT(" -U \"%s\"") , username) ;
	}

	if(domain != wxEmptyString)
	{
		cmd_line += wxString::Format(wxT(" -W \"%s\"") , domain) ;
		cmd_line += wxString::Format(wxT(" -n \"%s\"") , domain) ;
	}

	// Setup the remote working directory
	cmd_line += wxString::Format(wxT(" -D \"%s\"") , project_directory) ;
	wxString cmd ;
	wxString appendix = wxEmptyString ;
	
	do
	{
		to.SetName( from.GetName() + appendix ) ;
		to.SetExt ( from.GetExt() );

		cmd = wxString::Format("%s -c \"ls %s\"", cmd_line, to.GetFullName() );
		wxGetApp().Log(cmd);

		wxArrayString output;
		bool filename_exists = true;
		int code = wxExecute(cmd, output);
		
		if ( code != -1 )
		{
			// search for the STATUS_NO_SUCH_FILE
			unsigned int i ;
			for(i=0; i<output.GetCount(); i++)
			{
				wxGetApp().Log(output[i]);
				if(output[i].Find( wxT("STATUS_NO_SUCH_FILE")) != wxNOT_FOUND )
				{
					// A file with same name found on remote storage
					filename_exists = false ;
					break ;
				}
			}

			if(filename_exists)
			{
				appendix = wxString::Format(wxT("_%u%04u"), wxDateTime::GetTimeNow() , rand()%10000);
			}
			else
			{
				wxGetApp().Log("File not exists");
				break;
			}
			wxGetApp().Log(appendix);
		}
		else
		{
			return false;
		}
	}
	while(true);

	cmd = wxString::Format("%s -c \"put %s %s\"", cmd_line, from.GetFullPath(), to.GetFullName() );
	wxGetApp().Log(cmd);

	return true ;
}

bool DataFileStorage::OnTransferTerminate(int pid, int status, const wxString & datafile, const wxString & dest)
{
	return true ;
}

bool DataFileStorage::OnValidateTerminate(int pid, int status, const wxString & datafile, const wxString & dest)
{
	return true ;
}

bool DataFileStorage::AddTask(const wxFileName & from, const wxFileName & to) 
{
	return true ;
}
