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


AsyncProcess::AsyncProcess() : wxProcess::wxProcess()
{
	Redirect();
	Connect( wxEVT_TIMER , wxTimerEventHandler( AsyncProcess::OnTimer ));

	m_idleTimer = new wxTimer(this, 10) ;
	m_idleTimer->Start();

}

AsyncProcess::~AsyncProcess()
{
	Disconnect( wxEVT_TIMER , wxTimerEventHandler( AsyncProcess::OnTimer ));
	wxDELETE(m_idleTimer);
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
	if ( IsInputAvailable() )
	{
		wxTextInputStream tis(*GetInputStream());

		// this assumes that the output is always line buffered
		m_stdout.Add( tis.ReadLine() );
	}

	if ( IsErrorAvailable() )
	{
		wxTextInputStream tis(*GetErrorStream());

		// this assumes that the output is always line buffered
		wxString msg;
		m_stderr.Add( tis.ReadLine() );
	}
}