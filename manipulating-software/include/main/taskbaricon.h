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

#ifndef INCLUDED_MAIN_TASKBARICON_H
#define INCLUDED_MAIN_TASKBARICON_H

#include <wx/taskbar.h>

/**
 * Override the TaskBarIcon class
 * Presenting a icon in tray
 */
class MainTaskBarIcon: public wxTaskBarIcon
{
private:

public:
#if defined(__WXCOCOA__)
    MainTaskBarIcon(wxTaskBarIconType iconType = DEFAULT_TYPE) : wxTaskBarIcon(iconType)
#else
    MainTaskBarIcon()
#endif
    {}

    ~MainTaskBarIcon();
    void OnLeftButtonDClick(wxTaskBarIconEvent&);
    void OnMenuRestore(wxCommandEvent&);
    void OnMenuHide(wxCommandEvent&);
    void OnMenuExit(wxCommandEvent&);
    virtual wxMenu *CreatePopupMenu();

DECLARE_EVENT_TABLE()
};

#endif // INCLUDED_MAIN_TASKBARICON_H
