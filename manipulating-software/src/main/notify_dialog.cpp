///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 30 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "main/notify_dialog.h"

///////////////////////////////////////////////////////////////////////////
NotifyDialog * NotifyDialog::m_pInstance = NULL;


NotifyDialog * NotifyDialog::Get()
{
	if( ! m_pInstance)
	{
		m_pInstance = new NotifyDialog;
	}
	return m_pInstance ;
}



NotifyDialog::NotifyDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer;
	fgSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer->AddGrowableCol( 1 );
	fgSizer->AddGrowableRow( 0 );
	fgSizer->SetFlexibleDirection( wxBOTH );
	fgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_icon = new wxStaticBitmap( this, wxID_ANY, wxArtProvider::GetBitmap( wxART_INFORMATION, wxART_OTHER ), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer->Add( m_icon, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_TOP|wxALL, 5 );
	
	m_labelNotification = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_labelNotification->Wrap( -1 );
	fgSizer->Add( m_labelNotification, 0, wxALL|wxEXPAND, 10 );
	
	
	fgSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnClose = new wxButton( this, wxID_OK, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer->Add( m_btnClose, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	this->SetSizer( fgSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_btnClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NotifyDialog::OnClick ), NULL, this );
}

void NotifyDialog::Notify(const wxString & session_file)
{
	wxString text = wxString::Format(NOTIFY_TEXT, session_file);
	m_labelNotification->SetLabelMarkup (text);
	Show(true);
	SetFocus();
}

NotifyDialog::~NotifyDialog()
{
	// Disconnect Events
	m_btnClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NotifyDialog::OnClick ), NULL, this );
	
}

void NotifyDialog::OnClick( wxCommandEvent& event )
{
	Show(false);
}
