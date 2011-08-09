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

#ifndef INCLUDED_MAIN_DIALOG_H
#define INCLUDED_MAIN_DIALOG_H

#include <wx/dialog.h>
#include "main/taskbaricon.h"

class MainDialog : public wxDialog
{
private:
    DECLARE_CLASS(MainDialog)
    DECLARE_EVENT_TABLE()

    MainTaskBarIcon *taskbaricon;

protected:
    bool OnInit();

public:
    MainDialog() ;
    MainDialog(
            wxWindow* parent,
            wxWindowID id,
            const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_DIALOG_STYLE,
            const wxString& name = _T("")
            ) ;
    ~MainDialog() ;
};
 
 
#endif // INCLUDED_MAIN_DIALOG_H

