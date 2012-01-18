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
#include <wx/tokenzr.h>

#include "main/datarow.h"
 
DataRow::DataRow(const wxChar & delimiter)
{
	m_delimiter = delimiter ;
}

void DataRow::Import(const wxString & line)
{
	m_data = line ;
}

void DataRow::Import(const wxArrayString & ary)
{
	m_data = wxJoin(ary, m_delimiter) ;
}

wxArrayString DataRow::ToArrayString()
{
	return wxStringTokenize(m_data, m_delimiter);
}

bool DataRow::IsComment()
{
	if( m_data.Length()==0 )
	{
		return true ;
	}
	else
	{
		return m_data.GetChar(0)==wxT('#');
	}
}

wxChar DataRow::GetDelimiter()
{
	return m_delimiter ;
}

wxString DataRow::GetRowString()
{
	return m_data ;
}

wxString DataRow::ToASTAR()
{
	return m_data ;
}

wxString DataRow::ToHKL()
{
	return m_data ;
}

wxString DataRow::ToTSL()
{
	return m_data ;
}