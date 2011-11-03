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

#ifndef INCLUDE_MAIN_DIALOG_H
#define INCLUDE_MAIN_DIALOG_H

#define MAINDIALOG_TITLE _T("Console - Crystal Orientation Data Collection")

// The maximum numbers of messages to be listed in log window
#define MAINDIALOG_MESSAGE_COUNT 150

#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/listbox.h>
#include <wx/stattext.h>
#include <wx/button.h> 
#include <wx/sizer.h> 
#include <wx/event.h> 

class MainDialog : public wxDialog
{
private:
    DECLARE_CLASS(MainDialog)
    DECLARE_EVENT_TABLE()

    void set_properties();
    void set_layout();
    void set_event_handler();

protected:
    wxStaticText* labelTitle;
    wxStaticText* labelClear;
    wxListBox* listLogs;
    wxStaticText* labelSetting;
    wxButton* buttonClose;
    bool OnInit();

public:
    MainDialog() ;
    void OnClearLog(wxMouseEvent &);
    void OnSetting(wxMouseEvent &);
    void OnCloseClicked(wxCommandEvent &);
    bool AppendLog(const wxString& );
    ~MainDialog() ;
};
 
 
#endif // INCLUDE_MAIN_DIALOG_H

