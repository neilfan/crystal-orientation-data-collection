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

#include <wx/msgdlg.h>
#include <wx/config.h> 
#include <wx/fileconf.h> 
#include <wx/tokenzr.h> 

#include "main/server_connection.h" 
#include "main/process_controller.h"
#include "main/app.h"

//IMPLEMENT_CLASS( MainServerConnection, wxConnection )

bool MainServerConnection::OnExec(const wxString &topic, const wxString &data)
{
	return wxConnection::OnExec(topic, data) ;
}

bool MainServerConnection::OnPoke(const wxString &topic, const wxString &item, const void *data, size_t size, wxIPCFormat  format)
{
	// if an equipment is specified, start monitoring this equipment
	wxString equipment_id(item);
	wxGetApp().Log(wxT("Received a request to launch equipment ") + item);
	if( item == EQUIPMENT_NULL )
	{
		wxGetApp().Log(wxT("Invalid Equipment. Loading default equipment from configuration"));
		wxStringTokenizer tokenizer( wxConfig::Get()->Read( wxT("equipment.list")), ",");
		while ( tokenizer.HasMoreTokens() )
		{
			equipment_id = tokenizer.GetNextToken();
			break;
		}
	}
	
	if( wxConfig::Get()->Read( wxT("equipment.") + equipment_id + ".id" ) == wxEmptyString )
	{
		wxGetApp().Log(wxT("Equipment ") + equipment_id + wxT(" not defined"));
	}
	else
	{
		wxGetApp().Log(wxT("Start session confirmation for equipment ") + equipment_id);
		ProcessController::Get()->ConfirmNewSession(equipment_id);
	}
	return wxConnection::OnPoke(topic, item, data, size, format) ;
}

const void* MainServerConnection::OnRequest(const wxString &topic, const wxString &item, size_t * size, wxIPCFormat format)
{
	return wxConnection::OnRequest(topic, item, size, format) ;
}

bool MainServerConnection::OnDisconnect(void)
{
	return true;
}