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

#include "main/datarow_ang.h"
#include "main/datarow_astar.h"


DataRowASTAR::DataRowASTAR()
{
}

wxString DataRowASTAR::ToHKL()
{

	float rad ;
	if( IsComment() )
	{
		return m_data ;
	}
	else
	{
		wxArrayString astar = ToArrayString();
		wxArrayString hkl ;

		if(astar.GetCount()<9)
		{
			return wxEmptyString ;
		}
		
		hkl.Add( wxString::Format("%d", wxAtoi(astar[7]))) ;
		hkl.Add( astar[3] ) ;
		hkl.Add( astar[4] ) ;
		hkl.Add( wxT("0") ) ;
		hkl.Add( wxT("0") ) ;

		// Euler to Radians
		rad = wxAtof(astar[0]) * M_PI / 180.0 ;
		hkl.Add( wxString::Format("%f", rad) ) ;

		// Euler to Radians
		rad = wxAtof(astar[1]) * M_PI / 180.0 ;
		hkl.Add( wxString::Format("%f", rad) ) ;

		if( DataRowASTAR::GetCrystalSystemType() == Cubic)
		{
			// Euler to Radians
			rad = wxAtof(astar[2]) * M_PI / 180.0 ;
			hkl.Add( wxString::Format("%f", rad) ) ;
		}
		if( DataRowASTAR::GetCrystalSystemType() == Hexagonal)
		{
			// Euler to Radians
			rad = round( ( wxAtof(astar[2]) + M_PI/2.0) * pow(10.0,3.0) ) / pow(10.0,3.0) * M_PI / 180.0 ;
			hkl.Add( wxString::Format("%f", rad) ) ;

		}
		hkl.Add( wxT("0") ) ;
		hkl.Add( wxString::Format("%f", round( wxAtof(astar[5]) )) );
		hkl.Add( wxString::Format("%f", wxAtof(astar[6]) * 100.f) );

		return wxJoin(hkl, wxT('\t'));
	}
}