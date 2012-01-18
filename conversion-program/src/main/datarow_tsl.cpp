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
#include "main/datarow_tsl.h"

float DataRowTSL::m_minIQ = 0.0 ;
float DataRowTSL::m_maxIQ = 0.0 ;


void DataRowTSL::SetIQRange(float min, float max)
{
	DataRowTSL::m_minIQ = min;
	DataRowTSL::m_maxIQ = max;
}

DataRowTSL::DataRowTSL()
{
}

wxString DataRowTSL::ToHKL()
{

	if( IsComment() )
	{
		return m_data ;
	}
	else
	{
		wxArrayString tsl = ToArrayString();
		wxArrayString hkl ;

		if(tsl.GetCount()<9)
		{
			return wxEmptyString ;
		}
		float g = wxAtof(tsl[6]) ;
		
		// CTF-Phase Index
		float h = wxAtof(tsl[7]) ;
		hkl.Add( wxString::Format("%f", (g<0 ? 0 : (h==0 ? 1 : h) ))) ;
		
		hkl.Add( tsl[3] ) ;
		hkl.Add( tsl[4] ) ;

		hkl.Add( wxT("0") ) ;
		hkl.Add( wxT("0") ) ;
		
		float a=wxAtof(tsl[0]);
		hkl.Add( wxString::Format("%f", g<0 ? 0 : round( a * pow(10,3) ) / pow(10,3) )) ;

		float b=wxAtof(tsl[1]);
		hkl.Add( wxString::Format("%f", g<0 ? 0 : round( b * pow(10,3) ) / pow(10,3) )) ;

		
		float c=wxAtof(tsl[2]);
		if( DataRowTSL::GetCrystalSystemType() == Cubic)
		{
			hkl.Add( wxString::Format("%f", g<0 ? 0 : round(  c           * pow(10,3) ) / pow(10,3) )) ;
		}
		if( DataRowTSL::GetCrystalSystemType() == Hexagonal)
		{
			hkl.Add( wxString::Format("%f", g<0 ? 0 : round( (c + M_PI/2) * pow(10,3) ) / pow(10,3) )) ;
		}
		
		hkl.Add( wxString::Format("%f", (g<0 ? 0 : g)) );

		hkl.Add( wxString::Format("%f", round(255 * ( wxAtof(tsl[5]) - DataRowTSL::m_minIQ )/(DataRowTSL::m_maxIQ - DataRowTSL::m_minIQ)    )      ) );
		hkl.Add( wxT("0") ) ;

		return wxJoin(hkl, wxT('\t'));
	}
}