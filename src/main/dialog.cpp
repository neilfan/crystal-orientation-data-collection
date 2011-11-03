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
#include <wx/msgdlg.h> 
 
#include "main/dialog.h"
#include "icon.xpm"

IMPLEMENT_CLASS( MainDialog, wxDialog )

BEGIN_EVENT_TABLE(MainDialog, wxDialog)
END_EVENT_TABLE()

MainDialog::MainDialog()
{
	Create( (wxWindow*) NULL, -1, MAINDIALOG_TITLE,
			wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxMINIMIZE_BOX | wxDIALOG_NO_PARENT  );
	OnInit();
}

MainDialog::~MainDialog()
{
}
 
bool MainDialog::OnInit()
{
	// define interface and layout
	labelTitle = new wxStaticText(this, wxID_ANY, wxT("System Logs"));
	labelClear = new wxStaticText(this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	listLogs = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE|wxLB_NEEDED_SB);
	labelSetting = new wxStaticText(this, wxID_ANY, wxT("Setting"));
	buttonClose = new wxButton(this, wxID_ANY, wxT("Close"));

	set_properties();
	set_layout();

	// binding events
	set_event_handler();

	// set up the task bar icon
	this->SetIcon(icon_xpm);

	return true;
}


void MainDialog::set_properties()
{
	labelClear->SetToolTip(wxT("Clear all system logs"));
	listLogs->SetMinSize(wxSize(600, 450));
	labelSetting->SetForegroundColour(wxColour(0, 0, 255));
	labelClear->SetForegroundColour(wxColour(0, 0, 255));
}


void MainDialog::set_layout()
{
	wxFlexGridSizer* sizerMain = new wxFlexGridSizer(3, 1, 0, 0);
	wxGridSizer* sizerTop = new wxGridSizer(1, 2, 0, 0);
	wxGridSizer* sizerBottom = new wxGridSizer(1, 2, 0, 0);
	wxGridSizer* sizerBottomLeft = new wxGridSizer(1, 2, 0, 0);

	sizerTop->Add(labelTitle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 6);

	sizerBottomLeft->Add(labelSetting, 0, wxALL|wxALIGN_CENTER_VERTICAL, 6);
	sizerBottomLeft->Add(labelClear, 0, wxALL|wxALIGN_CENTER_VERTICAL, 6);

	sizerBottom->Add(sizerBottomLeft, 0, wxALIGN_CENTER_VERTICAL, 6);
	sizerBottom->Add(buttonClose, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);

	sizerMain->Add(sizerTop, 1, wxEXPAND, 0);
	sizerMain->Add(listLogs, 1, wxALL|wxEXPAND, 2);
	sizerMain->Add(sizerBottom, 1, wxEXPAND|wxALIGN_BOTTOM, 0);

	SetSizer(sizerMain);
	sizerMain->Fit(this);
	Layout();
}

/**
 * Event Handler when clicked on "Close" button
 * Hide the dialog
 */
void MainDialog::OnCloseClicked(wxCommandEvent & event)
{
	this->Show(false);
}

/**
 * Event Handler when clicked on "Clear" label
 * Clear all logs in listbox
 */
void MainDialog::OnClearLog(wxMouseEvent& event)
{
	listLogs->Clear();
}

/**
 * Event Handler when clicked on "Setting" label
 * Show settings
 */
void MainDialog::OnSetting(wxMouseEvent& event)
{
}

bool MainDialog::AppendLog(const wxString & label)
{
	listLogs->Append( label );
	return true ;
}

void MainDialog::set_event_handler()
{
	// event binding for label "Clear"
	labelClear->Bind(wxEVT_LEFT_DOWN, &MainDialog::OnClearLog, this);

	// event binding for label "Setting"
	labelSetting->Bind(wxEVT_LEFT_DOWN, &MainDialog::OnSetting, this);

	// event binding for button "Close"
	buttonClose->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &MainDialog::OnCloseClicked, this );
}

