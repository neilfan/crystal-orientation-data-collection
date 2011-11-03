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

#ifndef INCLUDED_MAIN_CONNECTION_H
#define INCLUDED_MAIN_CONNECTION_H

#include <wx/ipc.h> 
/**
 * the connection
 * will be shared by both client and server
 */
class MainConnection : public wxConnection
{
public:
	virtual bool DoExecute(const void *data, size_t size, wxIPCFormat format);
	virtual const void *Request(const wxString& item, size_t *size = NULL, wxIPCFormat format = wxIPC_TEXT);
	virtual bool DoPoke(const wxString& item, const void* data, size_t size, wxIPCFormat format);
	virtual bool OnAdvise(const wxString& topic, const wxString& item, const void *data, size_t size, wxIPCFormat format);
	virtual bool OnDisconnect();
};

#endif
