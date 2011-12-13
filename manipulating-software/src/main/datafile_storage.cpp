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

#include "main/process_controller.h"
#include "main/datafile_storage.h"
#include "main/async_process.h"
#include "main/app.h"

/**
 * class ValidateProcess
 *
 */

// This is the handler for process termination events
class ValidateProcess : public AsyncProcess
{
protected:
	wxString m_datafile;
	wxString m_destination;

public:
	ValidateProcess(const wxString& datafile, const wxString & dest)
		:AsyncProcess(), m_datafile(datafile), m_destination(dest)
	{
	}
	
	const wxString & GetDataFile()
	{
		return m_datafile ;
	}

	const wxString & GetDestination()
	{
		return m_destination ;
	}

	// instead of overriding this virtual function we might as well process the
	// event from it in the frame class - this might be more convenient in some
	// cases
	virtual void OnTerminate(int pid, int status)
	{
		DataFileStorage::Get()
			->OnValidateTerminate(status, this) ;
	}

};

/**
 * class TransferProcess
 *
 */

// This is the handler for process termination events
//class TransferProcess : public wxProcess

class TransferProcess : public AsyncProcess
{
protected:
	wxString m_datafile;
	wxString m_destination;

public:
	TransferProcess(const wxString& datafile, const wxString & dest)
		:AsyncProcess(), m_datafile(datafile), m_destination(dest)
	{
	}

	const wxString & GetDataFile()
	{
		return m_datafile ;
	}

	const wxString & GetDestination()
	{
		return m_destination ;
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
	
	if(wxProcess::Exists(m_childPid))
	{
	}
		wxProcess::Kill(m_childPid, wxSIGKILL, wxKILL_CHILDREN ) ;
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
					m_isTransferring = true ;
					m_timer->Stop();
					wxGetApp().Log(
						wxString::Format("copy %s to %s", from, to)
					);
					
					Validate( from, to);
				}
			}
		}
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
	wxFileName from(datafile) ;
	wxFileName to(dest);
	
	wxString project_directory = to.GetPath(wxPATH_GET_SEPARATOR, wxPATH_UNIX );
	
	wxString cmd_line = wxString::Format("%s -D \"%s\" -c \"put %s %s\"", GetCommandLine(), project_directory, from.GetFullPath(), to.GetFullName() );
	wxGetApp().Log(wxString::Format("Transfer cmd_line %s", cmd_line));
	TransferProcess * process = new TransferProcess(from.GetFullPath(), to.GetFullPath(wxPATH_UNIX ));
	m_childPid = wxExecute(cmd_line, wxEXEC_ASYNC | wxEXEC_MAKE_GROUP_LEADER, process);
	wxGetApp().Log(wxString::Format("  PID %d", m_childPid));

	return true ;
}

bool DataFileStorage::Validate(const wxString & datafile, const wxString & dest, const wxString & appendix )
{
	wxFileName from(datafile) ;
	wxFileName to(dest);
	
	to.SetName( from.GetName() + appendix ) ;
	to.SetExt(  from.GetExt()  ) ;

	wxString project_directory = to.GetPath(wxPATH_GET_SEPARATOR, wxPATH_UNIX );
	
	wxString cmd_line = wxString::Format(wxT("%s -D \"%s\" -c \"ls %s\" ") , GetCommandLine(), project_directory, to.GetFullName() ) ;
	wxGetApp().Log(wxString::Format("Validate cmd_line %s", cmd_line));

	ValidateProcess * vprocess = new ValidateProcess(from.GetFullPath(), to.GetFullPath(wxPATH_UNIX ));
	m_childPid = wxExecute(cmd_line, wxEXEC_ASYNC, vprocess);
	wxGetApp().Log(wxString::Format("  PID %d", m_childPid));
	return true ;
}

bool DataFileStorage::OnTransferTerminate(int status, TransferProcess * process)
{
	wxString datafile = process->GetDataFile() ;
	wxString dest     = process->GetDestination() ;

	wxGetApp().Log(wxString::Format("Transfer Completed %s -> %s", datafile, dest));
	wxGetApp().Log(wxString::Format("Transfer Status %d", status));
	
	wxArrayString output = process->GetStdOutput() ;
	int i;
	for(i=0;i<output.GetCount();i++)
	{
		wxGetApp().Log(wxString::Format("OUT  %s", output[i]));
	}

	wxArrayString error = process->GetStdError() ;

	for(i=0;i<error.GetCount();i++)
	{
		wxGetApp().Log(wxString::Format("ERR  %s", error[i]));
	}

	return true ;
}

bool DataFileStorage::OnValidateTerminate(int status, ValidateProcess * process)
{
	wxString datafile = process->GetDataFile() ;
	wxString dest     = process->GetDestination() ;
	if(process->IsInputAvailable())
	{
		wxInputStream * s = process->GetInputStream() ;
		wxTextInputStream* output=new wxTextInputStream(*s);
		
		bool exists = true ;
		while(output->GetInputStream().CanRead())
		{
			wxString line = output->ReadLine();
			wxGetApp().Log(wxString::Format("OUTPUT %s", line));

			if(line.Find( wxT("STATUS_NO_SUCH_FILE")) != wxNOT_FOUND)
			{
				exists = false ;
			}
		}
		
		if(exists)
		{
			// a file with same name exists on remote storage
			wxString appendix = wxString::Format(wxT("_%u%04u"), wxDateTime::GetTimeNow() , rand()%10000);
			Validate( datafile, dest, appendix );
		}
		else
		{
			Transfer(datafile, dest) ;
		}
	}
	return true ;
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
