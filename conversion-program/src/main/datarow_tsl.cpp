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

		if(tsl.GetCount()<10)
		{
			return wxEmptyString ;
		}

		float rad ;
		float g = wxAtof(tsl[6]) ;
		
		// CTF-Phase Index
		int h = wxAtoi(tsl[7]) ;
		hkl.Add( wxString::Format("%d", (g<0.0 ? 0 : (h==0 ? 1 : h) ))) ;
		
		hkl.Add( tsl[3] ) ;
		hkl.Add( tsl[4] ) ;

		hkl.Add( wxT("0") ) ;
		hkl.Add( wxT("0") ) ;
		
		// Radians to Euler
		float a =  (g<0.0 ? 0.0 : round( wxAtof(tsl[0]) * pow(10.0,3.0) ) / pow(10.0,3.0)) ;
		rad = a * 180.0 / M_PI ;
		hkl.Add( wxString::Format("%f", rad) ) ;


		// Radians to Euler
		float b = ( g<0.0 ? 0.0 : round( wxAtof(tsl[1]) * pow(10.0,3.0) ) / pow(10.0,3.0) );
		rad = b * 180.0 / M_PI ;
		hkl.Add( wxString::Format("%f", rad )) ;

		
		float c;
		if( DataRowTSL::GetCrystalSystemType() == Cubic)
		{
			// Radians to Euler
			c = ( g<0.0 ? 0.0 : round(  wxAtof(tsl[2])            * pow(10.0,3.0) ) / pow(10.0,3.0) );
			rad = c * 180.0 / M_PI ;
			hkl.Add( wxString::Format("%f", rad )) ;
		}
		if( DataRowTSL::GetCrystalSystemType() == Hexagonal)
		{
			// Radians to Euler
			c = ( g<0.0 ? 0.0 : round( (wxAtof(tsl[2]) + M_PI/2.0) * pow(10.0,3.0) ) / pow(10.0,3.0) );
			rad = c * 180.0 / M_PI ;
			hkl.Add( wxString::Format("%f", rad )) ;
		}
		
		hkl.Add( wxString::Format("%f", (g<0.0 ? 0.0 : g)) );

		hkl.Add( wxString::Format("%f", round(255.0 * ( wxAtof(tsl[5]) - DataRowTSL::m_minIQ )/(DataRowTSL::m_maxIQ - DataRowTSL::m_minIQ)    )      ) );
		hkl.Add( wxT("0") ) ;

		return wxJoin(hkl, wxT('\t'));
	}
}