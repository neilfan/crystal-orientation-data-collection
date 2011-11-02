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

#include "main/connection.h"
#include "main/client.h"
#include "main/server.h"
#include "main/taskbaricon.h"
#include "main/dialog.h"


#include "process-controller/controller.h"


class MainApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();
	void ShowLogDialog(bool show);
	bool IsLogDialogShown();
	int ExitApplication();
private:
	MainServer * m_server  ;
	MainDialog * m_log_dialog ;
    MainTaskBarIcon * m_taskbaricon;

};
 
DECLARE_APP(MainApp)
 
#endif // INCLUDED_MAIN_APP_H
