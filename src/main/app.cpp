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

#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include <wx/icon.h>
#include <wx/taskbar.h>

#include "define.h"
#include "main/app.h"
#include "main/dialog.h"
 
IMPLEMENT_APP(MainApp)
 
// This is executed upon startup, like 'main()' in non-wxWidgets programs.
bool MainApp::OnInit()
{
	// check if another instance running
	m_singleton_checker = new wxSingleInstanceChecker(APP_NAME);
	if ( m_singleton_checker->IsAnotherRunning() )
	{
#if __WIN32__
		// locate the log window
		wxWindow* dialog = wxWindow::FindWindowByName(APP_UUID);
#else
		wxLogError(_("Another program instance is already running, aborting."));
#endif
		return false;
	}
	MainDialog *dialog = new MainDialog();
	SetTopWindow(dialog);

	dialog->Show(true);

	return true;
}

int MainApp::OnExit()
{
	delete m_singleton_checker;

	return 0;
}