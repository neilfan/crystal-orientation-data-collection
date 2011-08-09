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

#include <wx/wx.h>
#include "main/taskbaricon.h"

enum {
    PU_RESTORE = 10001,
    PU_HIDE,
    PU_EXIT
};


BEGIN_EVENT_TABLE(MainTaskBarIcon, wxTaskBarIcon)
    EVT_TASKBAR_LEFT_DCLICK  (MainTaskBarIcon::OnLeftButtonDClick)
    EVT_MENU                 (PU_RESTORE  , MainTaskBarIcon::OnMenuRestore)
    EVT_MENU                 (PU_HIDE     , MainTaskBarIcon::OnMenuHide)
    EVT_MENU                 (PU_EXIT     , MainTaskBarIcon::OnMenuExit)
END_EVENT_TABLE()

MainTaskBarIcon::~MainTaskBarIcon()
{
    RemoveIcon();
}

void MainTaskBarIcon::SetDialog(wxDialog * d)
{
    dialog = d;
}

void MainTaskBarIcon::OnLeftButtonDClick(wxTaskBarIconEvent&)
{
    dialog->Show( !dialog->IsShown() );
}

void MainTaskBarIcon::OnMenuRestore(wxCommandEvent& )
{
    dialog->Show(true);
}

void MainTaskBarIcon::OnMenuExit(wxCommandEvent& )
{
    dialog->Close(false);
    dialog->Destroy();
}

void MainTaskBarIcon::OnMenuHide(wxCommandEvent& )
{
    dialog->Show(false);
}


// Overridables
wxMenu *MainTaskBarIcon::CreatePopupMenu()
{
    // Try creating menus different ways
    // TODO: Probably try calling SetBitmap with some XPMs here
    wxMenu *menu = new wxMenu;
    menu->Append(PU_RESTORE , _T("&Restore Console"));
    menu->Append(PU_HIDE    , _T("&Hide Console"));    
#ifndef __WXMAC_OSX__ /*Mac has built-in quit menu*/
    menu->AppendSeparator();
    menu->Append(PU_EXIT    , _T("E&xit"));
#endif
    return menu;
}


