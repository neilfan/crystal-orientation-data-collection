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

#ifndef INCLUDED_MAIN_DATAFILE_MONITOR_H
#define INCLUDED_MAIN_DATAFILE_MONITOR_H

#include <wx/fswatcher.h> 

/**
 * A Filesystem monitor which reports new data
 * cration during a session
 *
 * The monitor will start immiduately when a new
 * session is triggered. Normally that happens
 * when the "Launch" button is clicked.
 *
 * Implemented in a Singleton pattern.
 *
 */
class DataFileMonitor : public wxFileSystemWatcher
{
public:
	static DataFileMonitor * Get();

    /**
     * Reset this monitor
     */
	static void Reset();

	~DataFileMonitor();
	void OnFileSystemEvent(wxFileSystemWatcherEvent& event);
	void AddExtension(const wxString & ext) ;
	void Start();
	void Stop();
	void ClearExtensions();
	bool IsMoniotring() ;


private:
	DataFileMonitor();
	
	static DataFileMonitor      * m_pInstance ;
	wxArrayString               * m_arrayExts ;
	bool                          m_isMorniting ;

} ;

#endif
