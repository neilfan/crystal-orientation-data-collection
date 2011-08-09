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

#include <wx/icon.h>
#include "main/dialog.h"
#include "icon.xpm"

IMPLEMENT_CLASS( MainDialog, wxDialog )

BEGIN_EVENT_TABLE(MainDialog, wxDialog)
    EVT_PAINT(MainDialog::OnPaint)
END_EVENT_TABLE()

MainDialog::MainDialog()
{
    OnInit();
}

MainDialog::MainDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
{
    Create( parent, id, title, pos, size, style, name );
    OnInit();
}

MainDialog::~MainDialog()
{
    // delete the taskbar icon
    delete taskbaricon ;
}
 
bool MainDialog::OnInit()
{
    this->SetIcon(icon_xpm);

    taskbaricon = new MainTaskBarIcon();
    taskbaricon->SetIcon(icon_xpm);
    taskbaricon->SetDialog(this);

    return true;
}



