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

#include <wx/fileconf.h> 
#include <wx/process.h> 

#include "main/process_controller.h"
#include "main/macro_scheduler.h"
#include "main/app.h"

/**
 * class ExportProcess
 *
 */

// This is the handler for process termination events
class ExportProcess : public wxProcess
{
protected:
    wxString m_script;
	wxString m_datafile;

public:
    ExportProcess(const wxString& script, const wxString& datafile)
		:wxProcess(), m_script(script), m_datafile(datafile)
    {
    }

    // instead of overriding this virtual function we might as well process the
    // event from it in the frame class - this might be more convenient in some
    // cases
    virtual void OnTerminate(int pid, int status)
	{

		ProcessController::Get()
			->OnExportTerminate(pid, status, m_script, m_datafile) ;
	}

};







MacroScheduler * MacroScheduler::m_pInstance = NULL;

MacroScheduler::MacroScheduler()
{
}

MacroScheduler::~MacroScheduler()
{
}

/**
 * Return the instance
 * Singleton pattern.
 */
MacroScheduler * MacroScheduler::Get()
{
	if( ! m_pInstance)
	{
		m_pInstance = new MacroScheduler;
	}
	return m_pInstance ;
}

/**
 * Start the macro schedule
 */
bool MacroScheduler::Execute(const wxString & script, const wxString & datafile)
{
	wxString program = wxFileConfig::Get()->Read(wxT("sys.macroscheduler.program"));
	wxString cmd = wxString::Format(
					wxT("\"%s\" \"%s\" \"%s\""),
					program,
					script,
					datafile
					);

	wxGetApp().Log(wxString::Format("Export cmd %s", cmd));

	ExportProcess * const callback = new ExportProcess(script, datafile) ;
	long pid = wxExecute(cmd, wxEXEC_ASYNC, callback) ;
	
	return pid > 0 ;
}
