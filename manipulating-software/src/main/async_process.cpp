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
#include <wx/txtstrm.h> 

#include "main/async_process.h"
#include "main/app.h"


AsyncProcess::AsyncProcess(bool capture, bool hide) : wxProcess::wxProcess()
{
	m_capture = capture ;
	m_hide = hide ;

	if(m_capture)
	{
		// if you want to capture the output,
		// we need to redirect them to stream
		
		// and, this will also hide the window automatically
		Redirect();
	}

	Connect( wxEVT_TIMER , wxTimerEventHandler( AsyncProcess::OnTimer ));

	m_timer = new wxTimer(this, 50) ;
	m_timer->Start();
}

AsyncProcess::~AsyncProcess()
{
	Disconnect( wxEVT_TIMER , wxTimerEventHandler( AsyncProcess::OnTimer ));
	if(m_timer != NULL)
    {
        wxDELETE(m_timer);
    }
}

const wxArrayString & AsyncProcess::GetStdOutput()
{
	return m_stdout ;
}

const wxArrayString & AsyncProcess::GetStdError()
{
	return m_stderr ;
}

void AsyncProcess::OnTimer( wxTimerEvent& event )
{

	// if m_capture
	// Redirect() is called and no window displayed
	if( ! m_capture)
	{

		// Capture of output is not required?
		// we just need to hide the window as requested

		if( m_hide )
		{
			// so you want to hide the window?
			// let's go through all windows
			// and stop the timer when it's really hidden
			HWND h = ::GetTopWindow(0);
			while ( h )
			{
				DWORD pid;
				::GetWindowThreadProcessId( h,&pid);

				if ( GetPid() == pid )
				{
					// here h is the handle to the window
					::ShowWindow(h, SW_HIDE);
					break;
				}
				h = ::GetNextWindow( h , GW_HWNDNEXT);
			}
		}
	}
	else
	{
		// if not capture,
		// Redirect() is not called, stream of STDOUT/STDERR will always be NULL

		// Capture of output is required?
		// grab the stream, and put all strings to an array

		if ( GetInputStream()!=NULL)
		{
			wxTextInputStream tis(*GetInputStream());


			// this assumes that the output is always line buffered
			while( ! tis.GetInputStream().Eof() )
			{
				wxString msg = tis.ReadLine() ;
				if( msg == wxEmptyString )
				{
					break;
				}
				else
				{
					m_stdout.Add( msg );
//					wxGetApp().Log(wxString::Format("  STDOUT %s", msg ));
				}
			}
		}

		if ( IsErrorAvailable() )
		{
			wxTextInputStream tis(*GetErrorStream());

			// this assumes that the output is always line buffered
			while( ! tis.GetInputStream().Eof() )
			{
				wxString msg = tis.ReadLine() ;
				if( msg == wxEmptyString )
				{
					break;
				}
				else
				{
					m_stderr.Add( msg );
//					wxGetApp().Log(wxString::Format("  STDERR %s", msg ));
				}
			}
		}
	}
}
