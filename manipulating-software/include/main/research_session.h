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

#ifndef INCLUDED_MAIN_RESEARCH_SESSION_H
#define INCLUDED_MAIN_RESEARCH_SESSION_H

#include <wx/arrstr.h>
#include <wx/fileconf.h>

class ResearchSession
{
private:
	wxString          m_id ;
	wxArrayString *   m_files;
	wxFileConfig *    m_config;

	bool              Save();

public:
	ResearchSession * Load(const wxString & id);
	ResearchSession * Create(const wxString & id=wxEmptyString);
	ResearchSession(const wxString & id);
	~ResearchSession();
	const wxArrayString & GetAllDataFiles();
	wxString GetMetadata(const wxString &);
    bool AddDataFile(const wxString & path,const wxString & dest=wxEmptyString) ;
    bool SetMetadata(const wxString & key, const wxString & value) ;
};

#endif
