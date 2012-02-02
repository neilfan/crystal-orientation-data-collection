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
#ifndef INCLUDED_MAIN_TRANSFER_PROCESS_H
#define INCLUDED_MAIN_TRANSFER_PROCESS_H

#include <wx/process.h> 
#include <wx/regex.h> 
#include <wx/wfstream.h> 
#include <wx/txtstrm.h> 
#include <wx/fileconf.h> 
#include <wx/filename.h> 
#include <wx/arrstr.h> 
#include <wx/stream.h> 
#include <wx/string.h> 
#include <wx/tokenzr.h>


#include "main/async_process.h"
#include "main/app.h"

/**
 * class TransferProcess
 *
 */

// This is the handler for process termination events
//class TransferProcess : public wxProcess

class TransferProcess : public AsyncProcess
{
protected:
	wxFileName      m_file;
	wxFileName      m_dest;
	wxString        m_sessionId ;
	wxString        m_remoteDir ;
	wxArrayString   m_cmds ;
	unsigned long   m_posCmds ;

public:
	TransferProcess(const wxString & sessionId, const wxString & file, const wxString & dest) ;
	const wxFileName & GetFile();
	const wxFileName & GetDest();
	const wxString GetFirstCommand();
	const wxString GetNextCommand();
	const wxString & GetRemoteDir();
	const wxString & GetSessionId();
	virtual void OnTerminate(int pid, int status);
};

#endif