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

#ifndef INCLUDED_MAIN_CLIENT_H
#define INCLUDED_MAIN_CLIENT_H

#include <wx/ipc.h> 

#include "main/connection.h"

class MainClient: public wxClient
{
public:
	MainClient();
	~MainClient();
	bool Connect(const wxString& sHost, const wxString& sService, const wxString& sTopic);
	void Disconnect();
	wxConnectionBase *OnMakeConnection();
	bool IsConnected() { return m_connection != NULL; };
	MainConnection *GetConnection() { return m_connection; };

protected:
	MainConnection     *m_connection;
}; 
#endif
