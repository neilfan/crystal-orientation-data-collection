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

#ifndef INCLUDED_MAIN_SERVER_H
#define INCLUDED_MAIN_SERVER_H

#include <wx/ipc.h> 

#include "main/connection.h"

class MainServer: public wxServer
{
public:
	MainServer();
	~MainServer();
    void Disconnect();
    bool IsConnected() { return m_connection != NULL; }
    MainConnection *GetConnection() { return m_connection; }

    void Advise();
    bool CanAdvise() { return m_connection != NULL; }

    virtual wxConnectionBase *OnAcceptConnection(const wxString& topic);

protected:
    MainConnection *m_connection;
};
 
#endif
