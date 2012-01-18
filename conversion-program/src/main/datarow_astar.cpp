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
		
		hkl.Add( astar[7] ) ;
		hkl.Add( astar[3] ) ;
		hkl.Add( astar[4] ) ;
		hkl.Add( wxT("0") ) ;
		hkl.Add( wxT("0") ) ;
		hkl.Add( astar[0] ) ;
		hkl.Add( astar[1] ) ;
		if( DataRowASTAR::GetCrystalSystemType() == Cubic)
		{
			hkl.Add( astar[2] ) ;
		}
		if( DataRowASTAR::GetCrystalSystemType() == Hexagonal)
		{
			hkl.Add( wxString::Format("%f", round( ( wxAtof(astar[2]) + M_PI/2) * pow(10,3) ) / pow(10,3)));
		}
		hkl.Add( wxT("0") ) ;
		hkl.Add( wxString::Format("%f", round( wxAtof(astar[5]) )) );
		hkl.Add( wxString::Format("%f", wxAtof(astar[6]) * 100) );

		return wxJoin(hkl, wxT('\t'));
	}
}