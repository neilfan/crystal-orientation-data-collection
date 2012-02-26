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

#ifndef INCLUDED_MAIN_ASYNC_PROCESS_H
#define INCLUDED_MAIN_ASYNC_PROCESS_H

#include <wx/process.h> 
#include <wx/arrstr.h> 
#include <wx/timer.h> 


/**
 * Async Process
 * This class defines a async process which stands
 * for a execution of external program.
 *
 * When passed to wxExecute(), the execution will
 * not block the main workflow of whole app and
 * the application will be notified for the
 * termination of external program.
 *
 * If m_capture is set, stderr and stdout will be
 * captured and accessible by main application
 *
 * If m_hide is set, the external program will
 * not show any dialog/frame on screen. A timer
 * will be triger to force the output window hidden
 * from user.
 *
 */
class AsyncProcess : public wxProcess
{
protected:
	wxTimer          * m_timer ;
	wxArrayString      m_stdout ;
	wxArrayString      m_stderr ;
	size_t             m_lineCount ;
	bool               m_capture ;
	bool               m_hide ;

	void OnTimer( wxTimerEvent& event );

public:
	AsyncProcess(bool capture=true, bool hide=false);
	~AsyncProcess();
	const wxArrayString & GetStdOutput();
	const wxArrayString & GetStdError();
};

 
#endif
