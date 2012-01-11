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
#include <wx/txtstrm.h> 
#include <wx/wfstream.h>


#include "main/research_session.h"
#include "main/app.h"

ResearchSession * ResearchSession::Load(const wxString & id)
{
	ResearchSession * s = new ResearchSession(id);
	return s ;
}

ResearchSession * ResearchSession::Create(const wxString & id)
{
	wxString new_id = id ;
	if(id == wxEmptyString)
	{
		// TODO: create a config file
		new_id = wxDateTime::Now().Format(FILENAME_DATETIME_FORMAT) ;
		wxFileName filename("sessions", new_id, "ini") ;
		wxFile file;
		file.Create(filename.GetFullPath(), true);
	}
	ResearchSession * session = ResearchSession::Load(new_id);
	return session ;
}

ResearchSession::ResearchSession(const wxString & id)
{
	m_id = id ;
	m_files = NULL ;

	wxFileName filename("sessions", id, "ini") ;
	if( filename.IsFileReadable() )
	{
		wxFFileStream stream(filename.GetFullPath());
		m_config = new wxFileConfig(stream);
	}
}

ResearchSession::~ResearchSession()
{
	wxDELETE(m_files);
	wxDELETE(m_config);
}

const wxArrayString & ResearchSession::GetAllDataFiles()
{
	if( m_files == NULL )
	{
		m_files = new wxArrayString();
	
		// Load file list
		long count = m_config->ReadLong(wxT("files/count"), 0) ;
		long i;
		for(i=1; i<=count; i++)
		{
			// where is the original file
			wxString file_i = m_config->Read(
				wxString::Format("files/file%d", i),
				wxEmptyString);
			wxString dest_i = m_config->Read(
				wxString::Format("files/destination%d", i),
				wxEmptyString);
			
			m_files->Add(file_i + wxT("|") + dest_i);
		}
	}
	return * m_files;
}

wxString ResearchSession::GetMetadata(const wxString & item)
{
	return m_config->Read(wxT("metadata/") + item, wxT("")) ;
}

bool ResearchSession::AddDataFile(const wxString & path, const wxString & dest) 
{
	long seq = m_config->ReadLong(wxT("files/count"), 0) + 1 ;
	m_config->Write(
		wxString::Format("files/file%d", seq),
		path);
	m_config->Write(
		wxString::Format("files/destination%d", seq),
		dest);
	m_config->Write(wxT("files/count"), 
		wxString::Format("%d", seq)
		) ;

	return Save();
}

bool ResearchSession::SetMetadata(const wxString & key, const wxString & value)
{
	m_config->Write(wxT("metadata/") + key, value);
	
	return Save();

}

bool ResearchSession::Save()
{
	wxFileName filename("sessions", m_id, "ini") ;
	if( filename.IsFileWritable() )
	{
		wxFFileStream stream(filename.GetFullPath());
		return m_config->Save(stream) && m_config->Flush();
	}
	return false;
}

