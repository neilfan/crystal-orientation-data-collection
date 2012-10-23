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
#ifndef INCLUDED_MAIN_PROJECT_UPDATER_H
#define INCLUDED_MAIN_PROJECT_UPDATER_H

#define PROJECT_FILENAME          wxT("PROJECT.ini")
#define PROJECT_LIST_FILENAME     wxT("PROJECT_LIST.txt")
#define INTERVAL_DAY              86400000 // millisecond

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

class UpdateProcess ;
class ProjectUpdater : public wxEvtHandler 
{
public:

	static ProjectUpdater * Get();
	static void Reset();



	~ProjectUpdater();
	bool Start();
	wxString GetCommandLine(const wxString & cmd=wxEmptyString) ;
	void Update();
	bool IsBusy();
private:
	static ProjectUpdater * 	m_pInstance ;
	wxTimer * 					m_timer ;
	wxString 					m_cmd ;
	wxArrayString * 			m_projectIds ;

	bool 						m_isBusy ;
	ProjectUpdater();
	void OnTimer( wxTimerEvent& event ) ;
	bool OnCommandTerminate(int status, UpdateProcess * process);

	// assign permission to UpdateProcess class
	friend class UpdateProcess ;

};


#endif
