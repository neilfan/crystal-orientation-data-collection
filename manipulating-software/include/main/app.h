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
 
#define APP_NAME wxT("DC20A::CRYSTAL_ORIENTATION_DATA_COLLECTION")
#define DDE_TOPIC wxT("DC20A")
#define EQUIPMENT_NULL wxT("NO-NAME-SPECIFIED")
#define DATETIME_FORMAT_DEFAULT wxT("%Y%m%d-%H%M%S")
#define DATETIME_FORMAT_DATE wxT("%Y%m%d")

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif


/**
 * Main Application class
 *
 * This class will register the application and 
 * provide a static method for Logging
 */
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

	 /**
	  * Get config file name
	  * @return wxString the full path of config name
	  */
	wxString GetConfigFileName() ;

	 /**
	  * Launch a equipment
	  * @return wxString the full path of config name
	  */
	void ProcessEquipmentRequest(const wxString &) ;

private:
    /**
     * a pointer to the Server object
     */
	wxObject * m_server ;

	wxObject * m_taskbaricon;
	wxString   m_config_file_name ;
	wxString   m_log_file_name ;
	FILE     * m_log_fp;
};

DECLARE_APP(MainApp)
 
#endif // INCLUDED_MAIN_APP_H
