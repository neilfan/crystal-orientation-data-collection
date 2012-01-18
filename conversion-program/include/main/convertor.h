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
#ifndef INCLUDED_MAIN_CONVERTOR_H
#define INCLUDED_MAIN_CONVERTOR_H

#define COLUMNLENGTH_ASTAR 9
#define COLUMNLENGTH_TSL   10
#define FILENAMEEXT_ANG    wxT("ang")
#define FILENAMEEXT_CTF    wxT("ctf")

#include <wx/object.h>
#include <wx/textfile.h>


class Convertor : public wxObject
{
public:
	enum Format
	{
		FORMAT_UNKNOW,
		FORMAT_ASTAR,
		FORMAT_HKL,
		FORMAT_TSL
	};
	static Convertor * Get();
	~Convertor();
	bool LoadDataFile(const wxString &) ;
	Convertor::Format DetermineFormat() ;
	bool ToAstar(const wxString & output=wxEmptyString) ;
	bool ToHkl  (const wxString & output=wxEmptyString) ;
	bool ToTsl  (const wxString & output=wxEmptyString) ;
	int  GetProgress();
	wxArrayString GetGeneratedFiles() ;
private:
	Convertor();
	bool Convert(Convertor::Format format, const wxString & output=wxEmptyString) ;
	static Convertor * m_pInstance ;
	wxTextFile * m_textfile ;
	int m_progress ;
	wxArrayString m_generated_files ;

} ;

#endif