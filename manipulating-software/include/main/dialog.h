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

/**
 * This file is sourced from the export code from wxFormBuilder,
 * Refer to doc/UIDesign.wxFormBuilder.fbp (wxFormBUilder file)
 */

#ifndef INCLUDE_MAIN_DIALOG_H
#define INCLUDE_MAIN_DIALOG_H

// The maximum numbers of messages to be listed in log window
#define MAINDIALOG_MESSAGE_COUNT 150

#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/listbox.h>
#include <wx/stattext.h>
#include <wx/button.h> 
#include <wx/sizer.h> 
#include <wx/event.h> 

/**
 * The log dialog
 *
 * Refer to doc/ folder for the UI design
 * and the code genetator
 */
class MainDialog : public wxDialog
{
private:

    void set_properties();
    void set_layout();
    void set_event_handler();

protected:
	wxListBox* m_listLogs;
	wxStaticText* m_labelSetting;
	wxStaticText* m_labelClear;
	wxButton* m_buttonClose;
	
	// Virtual event handlers, overide them in your derived class
	virtual void OnSetting( wxMouseEvent& event ) ;
	virtual void OnClearLog( wxMouseEvent& event ) ;
	virtual void OnCloseClicked( wxCommandEvent& event );

	static MainDialog * m_pInstance ;

	MainDialog( 
		wxWindow* parent=NULL,
		wxWindowID id = wxID_ANY,
		const wxString& title = wxT("DC20A: Manipulating Software"), 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxSize( -1,-1 ), 
		long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU
	); 
public:
	~MainDialog();

	static MainDialog * Get();

    bool AppendLog(const wxString& );
};
 
 
#endif // INCLUDE_MAIN_DIALOG_H

