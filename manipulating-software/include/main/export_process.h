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

#ifndef INCLUDED_MAIN_EXPORT_PROCESS_H
#define INCLUDED_MAIN_EXPORT_PROCESS_H

#include "main/async_process.h"



/**
 * class ExportProcess
 *
 * Stands for a process performing customised
 * export action.
 *
 * An export action will integrate with the
 * Equipment (microscope in DC20A) to generate
 * data based on the raw data.
 *
 * Export is an option action and may not
 * available for all equipment.
 */

class ExportProcess : public AsyncProcess
{
protected:
	wxString        m_sessionId ;

public:
	ExportProcess(const wxString & sessionId);
	
	const wxString & GetSessionId();

	virtual void OnTerminate(int pid, int status) ;
};


#endif
