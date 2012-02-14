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

#include "main/transfer_process.h"
#include "main/datafile_storage.h"


TransferProcess::TransferProcess(const wxString & sessionId, const wxString & file, const wxString & dest) : AsyncProcess(false, true)
{
	// load remote project directory
	wxFileName session_ini("sessions", sessionId, "ini");
	if( ! session_ini.FileExists() )
	{
		wxFileConfig session_config( 
			wxEmptyString,
			wxEmptyString, 
			session_ini.GetFullPath(),
			wxEmptyString,
			wxCONFIG_USE_LOCAL_FILE|wxCONFIG_USE_NO_ESCAPE_CHARACTERS
		);

		wxString project_id = session_config.Read(wxT("metadata/session.project.id"), wxEmptyString);
		wxString equipment_id = session_config.Read(wxT("metadata/session.equipment.id"), wxEmptyString);
		if( project_id!=wxEmptyString)
		{
			m_remoteDir = wxFileConfig::Get()->Read(wxT("sys.network.smb.storage")) ;
			m_remoteDir.Replace(wxT("%PID%"), project_id) ;
			m_remoteDir.Replace(wxT("%EID%"), equipment_id) ;
		}
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

const wxFileName & TransferProcess::GetFile()
{
	return m_file ;
}

const wxFileName & TransferProcess::GetDest()
{
	return m_dest ;
}

const wxString TransferProcess::GetFirstCommand()
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

const wxString TransferProcess::GetNextCommand()
{
	m_posCmds++ ;
	if(!m_cmds.IsEmpty() && m_posCmds < m_cmds.GetCount() )
	{
		return m_cmds[m_posCmds] ;
	}

	return wxEmptyString ;
}

const wxString & TransferProcess::GetRemoteDir()
{
	return m_remoteDir ;
}

const wxString & TransferProcess::GetSessionId()
{
	return m_sessionId;
}

// instead of overriding this virtual function we might as well process the
// event from it in the frame class - this might be more convenient in some
// cases
void TransferProcess::OnTerminate(int pid, int status)
{
	DataFileStorage::Get()->OnTransferTerminate(status, this) ;
}


