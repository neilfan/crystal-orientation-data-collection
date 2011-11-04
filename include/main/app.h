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

#ifndef INCLUDED_MAIN_APP_H
#define INCLUDED_MAIN_APP_H
 
#define APP_NAME _T("DC20A::CRYSTAL_ORIENTATION_DATA_COLLECTION")

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif


class MainApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();
	
	/**
	 * Display or hide the log dialog.
	 * @var bool show Display (true) or Hide (false)
	 */
	void ShowLogDialog(bool show);

	/**
	 * Determine if the log dialog is displayed on screen
	 * @return bool Shown (true) or Hide (false)
	 */
	bool IsLogDialogShown();
	
	/**
	 * Force the application to exit
	 */
	void ExitApplication();
	
	/**
	 * Append a new message to log list
	 */
	void Log(const wxString & string);

private:
	wxObject * m_server ;
	wxObject * m_taskbaricon;
	wxObject * m_log_dialog ;
	FILE * m_log_fp;
};
 
DECLARE_APP(MainApp)
 
#endif // INCLUDED_MAIN_APP_H