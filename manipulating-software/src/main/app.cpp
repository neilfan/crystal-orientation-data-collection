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

#include <wx/filefn.h> 
#include <wx/config.h> 
#include <wx/fileconf.h> 
#include <wx/stdpaths.h> 
#include <wx/wfstream.h> 
#include <wx/cmdline.h> 
#include <wx/tokenzr.h> 
#include <wx/dir.h>
#include <wx/base64.h>
#include <wx/buffer.h>


#include "icon.xpm"
#include "main/app.h"
#include "main/server.h"
#include "main/taskbaricon.h"
#include "main/dialog.h"
#include "main/client.h"
#include "main/process_controller.h"
#include "main/project_updater.h"

 
IMPLEMENT_APP(MainApp)
 
// This is executed upon startup, like 'main()' in non-wxWidgets programs.
bool MainApp::OnInit()
{
	/* initialize random seed: */
	srand ( time(NULL) );
	setlocale(LC_ALL,"");
	
	
	m_log_file_name = wxEmptyString ;

	wxLog::EnableLogging(false);
	
	// wxWidgets has a bug that first call to wxDateTime::Format will ignore the timezone
	// setting. Call here once for allignment
	wxDateTime::Today().Format(DATETIME_FORMAT_DATE);

	// Change working directory same as the binary for portable use
	wxSetWorkingDirectory( wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) );

	// Set up command line parameter parser
	wxCmdLineParser cmd_parser(wxAppConsole::argc , wxAppConsole::argv) ;
	cmd_parser.AddOption (wxT("c"), wxT("config-file"), wxT("Configuration File"));
	cmd_parser.AddOption (wxT("e"), wxT("equipment-id"), wxT("Equipment ID"));

	// Which equipment is associated?
	wxString equipment_id(EQUIPMENT_NULL);

	if(cmd_parser.Parse() == 0)
	{
		cmd_parser.Found(wxT("equipment-id"), & equipment_id) ;
	}
	
	MainClient * client = new MainClient();
	if( client->ValidHost( _T("localhost")) )
	{
		if(	client->Connect( _T("localhost"), APP_NAME, DDE_TOPIC))
		{
			client->GetConnection()->Poke(equipment_id, wxEmptyString);
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
			
			if(equipment_id != EQUIPMENT_NULL )
			{
				LaunchEquipment(equipment_id);
			}
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

	// Init the config file
	wxString config_file_name("config/default.ini");
	if(cmd_parser.Parse() == 0)
	{
		cmd_parser.Found(wxT("config-file"), &config_file_name) ;
	}

	Log(wxT("Seems this is the only instance. Starting application"));

	wxFileInputStream cfg_stream(config_file_name);
	if(cfg_stream.IsOk())
	{
		wxFileConfig * m_file_config = new wxFileConfig(cfg_stream);
		wxFileConfig::Set(m_file_config);
		Log(_T("Loading from config file ") + config_file_name);
		m_config_file_name = config_file_name ;

		wxArrayString projects ;
		if( wxDir::GetAllFiles(wxT("projects"), &projects, wxT("*.ini")) == 0)
		{
			ProjectUpdater::Get()->Update() ;
		}
		ProjectUpdater::Get()->Start() ;
	}
	else
	{
		Log(wxT("Failed to load config file ") + config_file_name);
	}
	
	// init the process controller
	// data transfer will start automatically from now on
	ProcessController::Get() ;

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
	Log(_T("Exiting application"));
	
	MainDialog * dialog = wxDynamicCast(m_log_dialog, MainDialog) ;
	// Close the top window, notify application to exit
	dialog->Close(false);

	// Some other objects to be cleared.
	dialog->Destroy();
	
	MainTaskBarIcon * taskbaricon = wxDynamicCast(m_taskbaricon, MainTaskBarIcon) ;
	taskbaricon->Destroy();

	// Remove the Process Controller
	ProcessController * controller = ProcessController::Get();
	wxDELETE(controller);

}

void MainApp::Log(const wxString & string)
{
	wxFileName log_file( "log", wxDateTime::Today().Format(DATETIME_FORMAT_DATE), "log");

	if( m_log_file_name==wxEmptyString || m_log_file_name!= log_file.GetFullName())
	{
		m_log_file_name = log_file.GetFullName() ;
		if( ! log_file.DirExists())
		{
			// create ./log directory if required
			log_file.Mkdir();
		}
		wxLog::EnableLogging(false);
		if(m_log_fp!=NULL)
		{
			fclose(m_log_fp);
		}

		m_log_fp = fopen(log_file.GetFullPath(), "a+");
		if(m_log_fp != NULL)
		{
			// Only log to file when the file is created
			wxLog * m_log = new wxLogStderr(m_log_fp) ;
			wxLog::SetActiveTarget(m_log);
			wxLog::SetLogLevel(wxLOG_Message );
			wxLog::SetLogLevel(wxLOG_Trace  );
			wxLog::EnableLogging(true);
			wxLog::DisableTimestamp();
		}
	}



	// Send string to log window
	wxString  msg = wxDateTime::Now().Format(DATETIME_FORMAT_DEFAULT) + wxT(" - ") + string ;
	wxDynamicCast(m_log_dialog, MainDialog)->AppendLog( msg) ;

	// send string to log file
	wxLogMessage(msg);
	wxLog::FlushActive();
}

wxString MainApp::GetConfigFileName()
{
	return m_config_file_name ;
}

void MainApp::LaunchEquipment(const wxString & equ_id)
{
	wxString equipment_id(equ_id);

	// if an equipment is specified, start monitoring this equipment
	Log(wxT("Received a request to launch equipment ") + equipment_id);
	if( equipment_id == EQUIPMENT_NULL )
	{
		Log(wxT("Invalid Equipment. Loading default equipment from configuration"));
		wxStringTokenizer tokenizer( wxConfig::Get()->Read( wxT("equipment.list")), ",");
		while ( tokenizer.HasMoreTokens() )
		{
			equipment_id = tokenizer.GetNextToken();
			break;
		}
	}
	
	if( wxConfig::Get()->Read( wxT("equipment.") + equipment_id + ".id" ) == wxEmptyString )
	{
		Log(wxT("Equipment ") + equipment_id + wxT(" not defined"));
	}
	else
	{
		Log(wxT("Start session confirmation for equipment ") + equipment_id);
		ProcessController::Get()->ConfirmNewSession(equipment_id);
	}

}