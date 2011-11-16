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

#include <wx/icon.h>
#include <wx/msgdlg.h> 
#include <wx/config.h> 
#include <wx/filename.h> 

#include "main/dialog.h"
#include "main/app.h"
#include "icon.xpm"

IMPLEMENT_CLASS( MainDialog, wxDialog )

BEGIN_EVENT_TABLE(MainDialog, wxDialog)
END_EVENT_TABLE()

MainDialog::MainDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* gridSizer;
	gridSizer = new wxFlexGridSizer( 4, 1, 0, 0 );
	gridSizer->AddGrowableCol( 0 );
	gridSizer->AddGrowableRow( 2 );
	gridSizer->SetFlexibleDirection( wxVERTICAL );
	gridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	gridSizer->Add( 0, 0, 1, wxALL|wxEXPAND, 5 );

	wxStaticText* labelTitle;
	labelTitle = new wxStaticText( this, wxID_ANY, wxT("System Logs"), wxDefaultPosition, wxDefaultSize, 0 );
	labelTitle->Wrap( -1 );
	
	gridSizer->Add( labelTitle, 0, wxALIGN_BOTTOM|wxALL|wxEXPAND, 5 );
	
	m_listLogs = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxSize( 450,300 ), 0, NULL, wxLB_ALWAYS_SB|wxLB_SINGLE ); 
	gridSizer->Add( m_listLogs, 0, wxALL|wxEXPAND|wxFIXED_MINSIZE, 5 );
	
	wxBoxSizer* boxSizer;
	boxSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_labelSetting = new wxStaticText( this, wxID_ANY, wxT("Setting"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labelSetting->Wrap( -1 );
	m_labelSetting->SetForegroundColour( wxColour( 0, 0, 255 ) );
	
	boxSizer->Add( m_labelSetting, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_labelClear = new wxStaticText( this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labelClear->Wrap( -1 );
	m_labelClear->SetForegroundColour( wxColour( 0, 0, 255 ) );
	
	boxSizer->Add( m_labelClear, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	boxSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_buttonClose = new wxButton( this, wxID_CANCEL, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	boxSizer->Add( m_buttonClose, 0, wxALL, 5 );
	
	gridSizer->Add( boxSizer, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( gridSizer );
	this->Layout();
	gridSizer->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_labelSetting->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainDialog::OnSetting ), NULL, this );
	m_labelClear->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainDialog::OnClearLog ), NULL, this );
	m_buttonClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialog::OnCloseClicked ), NULL, this );

		// set up icon
	this->SetIcon(icon_xpm);

}

MainDialog::~MainDialog()
{
	// Disconnect Events
	m_labelSetting->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainDialog::OnSetting ), NULL, this );
	m_labelClear->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainDialog::OnClearLog ), NULL, this );
	m_buttonClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialog::OnCloseClicked ), NULL, this );
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
	m_listLogs->Clear();
}

/**
 * Event Handler when clicked on "Setting" label
 * Show settings
 */
void MainDialog::OnSetting(wxMouseEvent& event)
{
	if( wxGetApp().GetConfigFileName() != wxEmptyString )
	{
		wxFileName file_name(wxGetApp().GetConfigFileName());
		wxLaunchDefaultApplication ( file_name.GetFullPath());
	}
	else
	{
		wxMessageBox( wxT("Config file not available. Check log for details"));
	}
}

bool MainDialog::AppendLog(const wxString & label)
{
	m_listLogs->Append( label );
	
	// only hold last MAINDIALOG_MESSAGE_COUNT messages
	int dif = m_listLogs->GetCount() - MAINDIALOG_MESSAGE_COUNT ;
	if(dif>0)
	{
		int i ;
		for(i=0; i< dif ; i++)
		{
			m_listLogs->Delete(0);
		}
	}
	
	m_listLogs->SetSelection( m_listLogs->GetCount()-1) ;
	return true ;
}

