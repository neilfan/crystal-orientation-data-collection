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

#ifndef INCLUDED_MAIN_DATAROW_TSL_H
#define INCLUDED_MAIN_DATAROW_TSL_H

#include <wx/string.h>
#include <wx/arrstr.h> 

#include "main/datarow_ang.h"

class DataRowTSL : public DataRowANG
{
private:
	static float m_minIQ ;
	static float m_maxIQ ;
public:
	DataRowTSL() ;
	virtual wxString ToHKL() ;
	static void SetIQRange(float min, float max) ;
} ;

#endif