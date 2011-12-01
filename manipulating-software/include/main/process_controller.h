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
#ifndef INCLUDED_MAIN_PROCESS_CONTROLLER_H
#define INCLUDED_MAIN_PROCESS_CONTROLLER_H

#define SESSION_FILENAME_METADATA wxT("METADATA")

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
 
class ProcessController
{
public:

	
	static ProcessController * Get();
	~ProcessController();

	/**
	 * Start Session Confirmation for a new session
	 */
	void ConfirmNewSession(const wxString & equipment_id);

	/**
	 * Start the equipment
	 */
	void StartNewSession(const wxString & exchange_file);

	bool OnNewDataFileFound(const wxString & file);

private:
	ProcessController();
	wxObject * m_confirm_dialog ;
	wxString m_current_session_id ;
	wxFileName GetCurrentSessionDirName() ;
	wxString ReadSessionMetaData(const wxString & key) ;
	bool LaunchEquipment();
	bool StartMonitoring();

	static ProcessController * m_pInstance ;
};


#endif