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

#include "main/export_process.h"
#include "main/process_controller.h"

ExportProcess::ExportProcess(const wxString& sessionId) : AsyncProcess(false, false)
{
	m_sessionId = sessionId ;
}

const wxString & ExportProcess::GetSessionId()
{
	return m_sessionId;
}

// instead of overriding this virtual function we might as well process the
// event from it in the frame class - this might be more convenient in some
// cases
void ExportProcess::OnTerminate(int pid, int status)
{

	ProcessController::Get()
		->OnExportTerminate(pid, status, this) ;
}