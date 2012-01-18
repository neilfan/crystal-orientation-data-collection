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

#include "main/app.h"
#include "main/convert_dialog.h"


IMPLEMENT_APP(MainApp)
 
// This is executed upon startup, like 'main()' in non-wxWidgets programs.
bool MainApp::OnInit()
{
	/* initialize random seed: */
	srand ( time(NULL) );
	setlocale(LC_ALL,"");

	wxLog::EnableLogging(false);

	// Change working directory same as the binary for portable use
	wxSetWorkingDirectory( wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) );


	// Set up command line parameter parser
	wxCmdLineParser cmd_parser(wxAppConsole::argc , wxAppConsole::argv) ;
	cmd_parser.AddSwitch (wxT("l"), wxT("launched-from-manipulating-software"));
	cmd_parser.AddOption (wxT("e"), wxT("research-exchange-file"), wxT("Exchange File"));


	ConvertDialog * dialog = new ConvertDialog();
	m_dialog = wxDynamicCast(dialog, wxObject) ;
	dialog->Show(true);
	SetTopWindow(dialog);

	wxString exchange_file ;
	if(cmd_parser.Parse() == 0)
	{
		if( cmd_parser.Found(wxT("launched-from-manipulating-software")) )
		{
			cmd_parser.Found(wxT("research-exchange-file"), & exchange_file) ;

			wxFileName ex(exchange_file);
			if( ex.FileExists() )
			{
				dialog->AutoConvert(ex);
				// an exchange file is found, turn into auto convert mode
			}
		}
		
	}
	

	return true;
}
