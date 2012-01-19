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
#include <math.h>
#include <wx/msgdlg.h>
#include <wx/regex.h>

#include "main/datarow_ang.h"
#include "main/datarow_hkl.h"


DataRowHKL::DataRowHKL() : DataRow(wxT('\t'))
{
}

bool DataRowHKL::IsComment()
{
	wxRegEx reg("^[0-9\t.]+$");
	return reg.Matches(m_data);
}

wxString DataRowHKL::ToASTAR()
{
	if( IsComment() )
	{
		return m_data ;
	}
	else
	{
		wxArrayString hkl = ToArrayString();
		wxArrayString astar ;

		if(hkl.GetCount()<11)
		{
			return wxEmptyString ;
		}
		
		astar.Add( hkl[5] ) ;
		astar.Add( wxString::Format("%f", wxAtof(hkl[6])/100) ) ;
		if( DataRowHKL::GetCrystalSystemType() == Cubic)
		{
			hkl.Add( hkl[7] ) ;
		}
		if( DataRowHKL::GetCrystalSystemType() == Hexagonal)
		{
			hkl.Add( wxString::Format("%f", ( wxAtof(hkl[2]) - M_PI/2) ));
		}
		astar.Add( hkl[8] ) ;
		astar.Add( hkl[1] ) ;
		astar.Add( hkl[2] ) ;
		astar.Add( hkl[9] ) ;
		astar.Add( hkl[10] ) ;
		astar.Add( hkl[0] ) ;

		return wxJoin(hkl, wxT('\t'));
	}
}