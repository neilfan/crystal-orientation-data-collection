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

#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/log.h> 
#include <wx/icon.h>
#include <wx/taskbar.h>

#include "main/app.h"
#include "icon.xpm"

 
IMPLEMENT_APP(MainApp)
 
// This is executed upon startup, like 'main()' in non-wxWidgets programs.
bool MainApp::OnInit()
{
	wxLog::EnableLogging(false);

	MainClient * client = new MainClient();
	if( client->ValidHost( _T("localhost")) )
	{
		if(	client->Connect( _T("localhost"), APP_NAME,	_T("TOPIC")	))
		{
			client->Disconnect();
			wxDELETE(client);
			return false;
		}
		else
		{
			wxDELETE(client);

			m_server = new MainServer();
			m_server->Create(APP_NAME);
		}
	}
	// Display the log window
	m_log_dialog = new MainDialog();
	SetTopWindow(m_log_dialog);
	ShowLogDialog(true);
	
	// Display taskbar icon
	m_taskbaricon = new MainTaskBarIcon();
	m_taskbaricon->SetIcon(icon_xpm);


	return true;
}

int MainApp::OnExit()
{
	if(m_server)
	{
		m_server->Disconnect();
		wxDELETE(m_server);
	}
	return 0;
}

void MainApp::ShowLogDialog(bool show)
{
	m_log_dialog->Show(show);
}

bool MainApp::IsLogDialogShown()
{
	return m_log_dialog->IsShown();
}


int MainApp::ExitApplication()
{
	// Close the top window, notify application to exit
	m_log_dialog->Close(false);
	
	// Some other objects to be cleared.
	m_log_dialog->Destroy();
	m_taskbaricon->Destroy();
	return 0;
}