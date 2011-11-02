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

#include "main/server.h"

MainServer::MainServer() : wxServer()
{
    m_connection = NULL;
}

MainServer::~MainServer()
{
    Disconnect();
}

wxConnectionBase *MainServer::OnAcceptConnection(const wxString& topic)
{
    wxMessageBox(topic);

    if ( topic == "TOPIC" )
    {
        m_connection = new MainConnection();
        wxMessageBox("Connection accepted");
        return m_connection;
    }
    //else: unknown topic

    wxMessageBox("Unknown topic, connection refused");
    return NULL;
}

void MainServer::Disconnect()
{
    if ( m_connection )
    {
        wxDELETE(m_connection);
    }
}

void MainServer::Advise()
{
    if ( CanAdvise() )
    {
        const wxDateTime now = wxDateTime::Now();

        m_connection->Advise(_T("NOW"), now.Format());

        const wxString s = now.FormatTime() + " " + now.FormatDate();
        m_connection->Advise(_T("TODAY"), s.mb_str(), wxNO_LEN);

#if wxUSE_DDE_FOR_IPC
        wxMessageBox("DDE Advise type argument cannot be wxIPC_PRIVATE. "
                     "The client will receive it as wxIPC_TEXT, "
                     " and receive the correct no of bytes, "
                     "but not print a correct log entry.");
#endif
        char bytes[3] = { '1', '2', '3' };
        m_connection->Advise(_T("OK"), bytes, 3, wxIPC_PRIVATE);
    }
}

