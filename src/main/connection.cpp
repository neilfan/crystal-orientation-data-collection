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

#include "main/connection.h"
 
bool MainConnection::OnAdvise(const wxString& topic, const wxString& item, const void *data,
	size_t size, wxIPCFormat format)
{
	wxMessageBox(topic);
	return true;
}

bool MainConnection::OnDisconnect()
{
	wxMessageBox(wxT("OnDisconnect()"));
	return true;
}

bool MainConnection::DoExecute(const void *data, size_t size, wxIPCFormat format)
{
	bool retval = wxConnection::DoExecute(data, size, format);
	if (!retval)
	{
		wxMessageBox(wxT("Execute failed!"));
	}
	return retval;
}

const void *MainConnection::Request(const wxString& item, size_t *size, wxIPCFormat format)
{
	const void *data =  wxConnection::Request(item, size, format);
	wxMessageBox(item);
	return data;
}

bool MainConnection::DoPoke(const wxString& item, const void *data, size_t size, wxIPCFormat format)
{
	wxMessageBox(item);
	return wxConnection::DoPoke(item, data, size, format);
}
