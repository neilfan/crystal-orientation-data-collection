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
#ifndef INCLUDED_MAIN_DATAFILE_STORAGE_H
#define INCLUDED_MAIN_DATAFILE_STORAGE_H

#define DATAFILE_STORAGE_TIMER_INTERVAL     1000
#define DATAFILE_STORAGE_CACHE_FILENAME     wxT("./CACHE")
#define DATAFILE_STORAGE_METADATA_FILENAME  wxT("METADATA.ini")
#define DATAFILE_STORAGE_LINE_DELIM         wxT(">")

#include <wx/wxprec.h>
#include <wx/filename.h> 
#include <wx/timer.h> 
#include <wx/arrstr.h>

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
 
class TransferProcess ;
class DataFileStorage : public wxEvtHandler 
{
public:
	static DataFileStorage * Get();

	~DataFileStorage();

	bool Start() ;
	bool Stop() ;
	bool AddTask(const wxFileName & from, const wxFileName & to) ;
	
	bool IsRunning()
	{
		return m_isTransferring || m_timer->IsRunning() ;
	}
	
	const wxString & GetCommandLine() ;
	bool Transfer(const wxString & datafile, const wxString & dest);



protected:
	static DataFileStorage * m_pInstance ;

	// Timer to check if any transfer task
	wxTimer * m_timer ;
	bool m_isTransferring ;
	long m_timer_interver ;
	
	// a string to record part of the command line
	wxString m_cmd ;
	
	wxArrayString m_sessionTasks ;
	wxString m_currentTransferSessionID ;

	void FinaliseSession(const wxString & );
	void OnTimer( wxTimerEvent& event ) ;
	bool OnTransferTerminate(int status, TransferProcess * process);
	DataFileStorage();


	// assign permission to TransferProcess/ValidateProcess class
	friend class TransferProcess ;
};


#endif