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


#include <wx/log.h> 

#include <wx/config.h> 
#include <wx/filefn.h> 
#include <wx/stdpaths.h> 
#include <wx/wfstream.h> 
#include <wx/fileconf.h> 

#include "icon.xpm"
#include "main/app.h"
#include "main/server.h"
#include "main/taskbaricon.h"
#include "main/dialog.h"
#include "main/client.h"

 
IMPLEMENT_APP(MainApp)
 
// This is executed upon startup, like 'main()' in non-wxWidgets programs.
bool MainApp::OnInit()
{
	wxLog::EnableLogging(false);

	// Change working directory same as the binary for portable use
	wxSetWorkingDirectory( wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) );



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

			MainServer * server = new MainServer();
			server->Create(APP_NAME);
			m_server = wxDynamicCast(server, wxObject);
		}
	}



	// Display the log window
	MainDialog * log_dialog = new MainDialog();
	SetTopWindow(log_dialog);
	log_dialog->Show();
	m_log_dialog = wxDynamicCast(log_dialog, wxObject) ;
	
	// Display taskbar icon
	MainTaskBarIcon * taskbaricon = new MainTaskBarIcon();
	taskbaricon->SetIcon(icon_xpm);
	m_taskbaricon = wxDynamicCast(taskbaricon, wxObject) ;

	// Setup the logging facility
	// Send logs to both log window and log file
	wxFileName log_file( "log", wxDateTime::Now().Format(_T("%Y-%m-%d-%H%M%S.log") ));
	if( ! log_file.DirExists())
	{
		// create ./log directory if required
		log_file.Mkdir();
	}
	m_log_fp = fopen(log_file.GetFullPath(), "w+");
	if(m_log_fp != NULL)
	{
		// Only log to file when the file is created
		wxLog * m_log = new wxLogStderr(m_log_fp) ;
		wxLog::SetActiveTarget(m_log);
		wxLog::SetLogLevel(wxLOG_Message );
		wxLog::EnableLogging(true);
		wxLog::DisableTimestamp();
	}

	// Init the config file
	wxFileInputStream cfg_stream(_T("crystal-orientation-data-collection.ini"));
	wxFileConfig * m_file_config = new wxFileConfig(cfg_stream);
	wxFileConfig::Set(m_file_config);
//	wxMessageBox( wxConfig::Get()->Read(_T("settings/app.owner")));

	Log(_T("Seems this is the only instance. Starting application"));

	return true;
}

int MainApp::OnExit()
{
	// shutdown the server (DDE)
	if(m_server!=NULL)
	{
		wxDynamicCast(m_server, MainServer)->Disconnect();
		wxDELETE(m_server);
	}

	// close log file handler
	if(m_log_fp!=NULL)
	{
		fclose(m_log_fp);
	}

	return 0;
}

void MainApp::ShowLogDialog(bool show)
{
	wxDynamicCast(m_log_dialog, MainDialog)->Show(show);
}

bool MainApp::IsLogDialogShown()
{
	return wxDynamicCast(m_log_dialog, MainDialog)->IsShown();
}


void MainApp::ExitApplication()
{
	MainDialog * dialog = wxDynamicCast(m_log_dialog, MainDialog) ;
	// Close the top window, notify application to exit
	dialog->Close(false);
	
	// Some other objects to be cleared.
	dialog->Destroy();
	
	MainTaskBarIcon * taskbaricon = wxDynamicCast(m_taskbaricon, MainTaskBarIcon) ;
	taskbaricon->Destroy();

}

void MainApp::Log(const wxString & string)
{
	wxString  msg = wxDateTime::Now().FormatISOCombined() + _T(" - ") + string ;
	wxDynamicCast(m_log_dialog, MainDialog)->AppendLog( msg) ;
	wxLogMessage(msg);
	wxLog::FlushActive();
}
