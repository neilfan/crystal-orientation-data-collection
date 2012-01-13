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

#include "icon.xpm"

#include "main/convert_dialog.h"
#include "main/datarow_format.h"

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 30 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////


ConvertDialog::ConvertDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Convert From:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer4->Add( m_staticText1, 0, wxALL|wxEXPAND, 5 );
	
	m_file_from = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file for conversion..."), wxT("ANG Files (*.ang)|*.ang|CTF Files (*.ctf)|*.ctf|All Files (*.*)|*.*"), wxPoint( -1,-1 ), wxSize( -1,-1 ), wxFLP_DEFAULT_STYLE|wxFLP_FILE_MUST_EXIST|wxFLP_OPEN );
	bSizer4->Add( m_file_from, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("The output file will be saved as:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer4->Add( m_staticText2, 0, wxALL|wxEXPAND, 5 );
	
	m_file_to = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer4->Add( m_file_to, 0, wxALL|wxEXPAND, 5 );
	
	m_progress = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxSize( -1,-1 ), wxGA_HORIZONTAL|wxGA_SMOOTH );
	m_progress->SetValue( 40 ); 
	bSizer4->Add( m_progress, 0, wxALL|wxEXPAND, 5 );
	
	m_sdbSizer4 = new wxStdDialogButtonSizer();
	m_sdbSizer4Apply = new wxButton( this, wxID_APPLY );
	m_sdbSizer4->AddButton( m_sdbSizer4Apply );
	m_sdbSizer4Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer4->AddButton( m_sdbSizer4Cancel );
	m_sdbSizer4->Realize();
	bSizer4->Add( m_sdbSizer4, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer4 );
	this->Layout();
	
	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ConvertDialog::OnClose ) );
	m_sdbSizer4Apply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConvertDialog::OnApply ), NULL, this );
	m_sdbSizer4Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConvertDialog::OnCancel ), NULL, this );

	// set up icon
	this->SetIcon(icon_xpm);

}

ConvertDialog::~ConvertDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ConvertDialog::OnClose ) );
	m_sdbSizer4Apply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConvertDialog::OnApply ), NULL, this );
	m_sdbSizer4Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConvertDialog::OnCancel ), NULL, this );
}

///////////////////////////////////////////////////////////////////////////

void ConvertDialog::OnApply(wxCommandEvent& event)
{
	// Do the convertion here
}

void ConvertDialog::OnCancel(wxCommandEvent& event)
{
	this->Close(true);
}

void ConvertDialog::OnClose(wxCloseEvent& event)
{
	this->Destroy();
}