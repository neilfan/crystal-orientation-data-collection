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

#ifndef INCLUDED_MAIN_CONVERTDATA_PROCESS_H
#define INCLUDED_MAIN_CONVERTDATA_PROCESS_H

#include "main/async_process.h"

class ConvertDataProcess : public AsyncProcess
{
protected:
	wxString        m_sessionId ;
	wxString        m_tmp_exchange ;

public:
	ConvertDataProcess(const wxString & sessionId, const wxString & tmp_exchange);

	const wxString & GetSessionId();
	const wxString & GetExchangeFile();

	// instead of overriding this virtual function we might as well process the
	// event from it in the frame class - this might be more convenient in some
	// cases
	virtual void OnTerminate(int pid, int status);

};

#endif