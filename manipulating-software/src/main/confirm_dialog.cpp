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

 ///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 30 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/tokenzr.h> 
#include <wx/icon.h>
#include <wx/wfstream.h> 
#include <wx/msgdlg.h> 
#include <wx/fileconf.h> 

#include "main/app.h"
#include "main/confirm_dialog.h"
#include "main/process_controller.h"
#include "icon.xpm"

#define PROJECT_CHOICE_TIP_DEFAULT wxT("Please identify your research project.")
#define PROJECT_CHOICE_TIP_NONE wxT("Research data will only be stored on local machine.")

///////////////////////////////////////////////////////////////////////////

ConfirmDialog::ConfirmDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_sizerMain = new wxFlexGridSizer( 5, 1, 0, 0 );
	m_sizerMain->AddGrowableCol( 0 );
	m_sizerMain->AddGrowableRow( 3 );
	m_sizerMain->SetFlexibleDirection( wxBOTH );
	m_sizerMain->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_panelBackground = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelBackground->SetBackgroundColour( wxColour( 255, 255, 128 ) );
	
	wxBoxSizer* sizerWarning;
	sizerWarning = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* sizerWarningMiddle;
	sizerWarningMiddle = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBitmap* bitmapWarning;
	bitmapWarning = new wxStaticBitmap( m_panelBackground, wxID_ANY, wxArtProvider::GetBitmap( wxART_WARNING, wxART_OTHER ), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	sizerWarningMiddle->Add( bitmapWarning, 0, 0, 0 );
	
	wxStaticText* labelWarning;
	labelWarning = new wxStaticText( m_panelBackground, wxID_ANY, wxT("Warning: Can not connect to server"), wxDefaultPosition, wxDefaultSize, 0 );
	labelWarning->Wrap( -1 );
	sizerWarningMiddle->Add( labelWarning, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	sizerWarning->Add( sizerWarningMiddle, 1, wxALIGN_CENTER, 0 );
	
	m_panelBackground->SetSizer( sizerWarning );
	m_panelBackground->Layout();
	sizerWarning->Fit( m_panelBackground );
	m_sizerMain->Add( m_panelBackground, 1, wxEXPAND | wxALL, 0 );
	
	
	m_sizerMain->Add( 0, 0, 1, wxALL|wxEXPAND, 10 );
	
	wxStaticText* labelTitle;
	labelTitle = new wxStaticText( this, wxID_ANY, wxT("Confirm Research Project"), wxDefaultPosition, wxDefaultSize, 0 );
	labelTitle->Wrap( -1 );
	
	m_sizerMain->Add( labelTitle, 0, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* sizerMiddle;
	sizerMiddle = new wxFlexGridSizer( 3, 3, 0, 0 );
	sizerMiddle->AddGrowableCol( 1 );
	sizerMiddle->AddGrowableRow( 2 );
	sizerMiddle->SetFlexibleDirection( wxBOTH );
	sizerMiddle->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticText* labelProject;
	labelProject = new wxStaticText( this, wxID_ANY, wxT("Research Project"), wxDefaultPosition, wxDefaultSize, 0 );
	labelProject->Wrap( -1 );
	sizerMiddle->Add( labelProject, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choiceProjectChoices[] = { wxEmptyString, wxT("- Not a registered research project -") };
	int m_choiceProjectNChoices = sizeof( m_choiceProjectChoices ) / sizeof( wxString );
	m_choiceProject = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceProjectNChoices, m_choiceProjectChoices, 0 );
	m_choiceProject->SetSelection( 0 );
	sizerMiddle->Add( m_choiceProject, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticText* staticText10;
	staticText10 = new wxStaticText( this, wxID_ANY, wxT("*"), wxDefaultPosition, wxDefaultSize, 0 );
	staticText10->Wrap( -1 );
	sizerMiddle->Add( staticText10, 0, wxALL, 5 );
	
	
	sizerMiddle->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_labelTips = new wxStaticText( this, wxID_ANY, wxT("Please identify your research project ."), wxDefaultPosition, wxDefaultSize, 0 );
	m_labelTips->Wrap( -1 );
	sizerMiddle->Add( m_labelTips, 0, wxALL|wxEXPAND, 5 );
	
	
	sizerMiddle->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxStaticText* staticTextMetadata;
	staticTextMetadata = new wxStaticText( this, wxID_ANY, wxT("Metadata"), wxDefaultPosition, wxDefaultSize, 0 );
	staticTextMetadata->Wrap( -1 );
	sizerMiddle->Add( staticTextMetadata, 0, wxALIGN_TOP|wxALL, 5 );
	
	m_gridMetadata = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxSize( 400,250 ), 0 );
	
	// Grid
	m_gridMetadata->CreateGrid( 0, 1 );
	m_gridMetadata->EnableEditing( true );
	m_gridMetadata->EnableGridLines( true );
	m_gridMetadata->EnableDragGridSize( false );
	m_gridMetadata->SetMargins( 0, 0 );
	
	// Columns
	m_gridMetadata->SetColSize( 0, 250 );
	m_gridMetadata->EnableDragColMove( false );
	m_gridMetadata->EnableDragColSize( true );
	m_gridMetadata->SetColLabelSize( 30 );
	m_gridMetadata->SetColLabelValue( 0, wxT("Value") );
	m_gridMetadata->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gridMetadata->EnableDragRowSize( false );
	m_gridMetadata->SetRowLabelSize( 250 );
	m_gridMetadata->SetRowLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gridMetadata->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	sizerMiddle->Add( m_gridMetadata, 0, wxALL|wxEXPAND, 5 );
	
	
	sizerMiddle->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_sizerMain->Add( sizerMiddle, 1, wxALL|wxEXPAND, 20 );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer4->AddGrowableCol( 0 );
	fgSizer4->AddGrowableRow( 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticText* staticText11;
	staticText11 = new wxStaticText( this, wxID_ANY, wxT("* Mandatory item"), wxDefaultPosition, wxDefaultSize, 0 );
	staticText11->Wrap( -1 );
	staticText11->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DDKSHADOW ) );
	
	fgSizer4->Add( staticText11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 10 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonCancel = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_buttonCancel, 0, wxALL, 5 );
	
	m_buttonLaunch = new wxButton( this, wxID_ANY, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonLaunch->SetDefault(); 
	bSizer5->Add( m_buttonLaunch, 0, wxALL, 5 );
	
	fgSizer4->Add( bSizer5, 1, wxALL|wxEXPAND, 5 );
	
	m_sizerMain->Add( fgSizer4, 1, wxEXPAND, 5 );
	
	this->SetSizer( m_sizerMain );
	this->Layout();
	m_sizerMain->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_choiceProject->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ConfirmDialog::OnProjectChoice ), NULL, this );
	m_gridMetadata->Connect( wxEVT_SIZE, wxSizeEventHandler( ConfirmDialog::OnSize ), NULL, this );
	m_buttonLaunch->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfirmDialog::OnLaunch ), NULL, this );


	/**
	 * Extra code not imported from wxFormBuilder
	 * Make sure not overwritten by update on form builder
	 */
	// set up icon
	this->SetIcon(icon_xpm);

	// load research projects into dropdown list
	wxStringTokenizer tokenizer( wxFileConfig::Get()->Read(wxT("project.list")), ",");
	while ( tokenizer.HasMoreTokens() )
	{
		wxString token = tokenizer.GetNextToken();
		m_choiceProject->Append(token);
	}



}

ConfirmDialog::~ConfirmDialog()
{
	// Disconnect Events
	m_choiceProject->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ConfirmDialog::OnProjectChoice ), NULL, this );
	m_gridMetadata->Disconnect( wxEVT_SIZE, wxSizeEventHandler( ConfirmDialog::OnSize ), NULL, this );
	m_buttonLaunch->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfirmDialog::OnLaunch ), NULL, this );
}



void ConfirmDialog::OnProjectChoice( wxCommandEvent& event )
{
	ResetGridMetadata();
	if(m_choiceProject->GetSelection()==0)
	{
		m_labelTips->SetLabelText(PROJECT_CHOICE_TIP_DEFAULT);
	}
	else if(m_choiceProject->GetSelection()==1)
	{
		m_labelTips->SetLabelText(PROJECT_CHOICE_TIP_NONE);
	}
	else
	{
		// Set the tips as project name
		m_labelTips->SetLabelText(
			wxFileConfig::Get()->Read(
				wxT("project.")
				+ m_choiceProject->GetStringSelection()
				+ wxT(".name")
			)
		);
		
		// load project metadata
		wxString config_file_name = wxFileConfig::Get()->Read(
				wxT("project.")
				+ m_choiceProject->GetStringSelection()
				+ wxT(".metadata.sourcefile")
			) ;
		wxFileInputStream cfg_stream(config_file_name);
		if(cfg_stream.IsOk())
		{
			wxGetApp().Log(_T("Loading project metadata list ") + config_file_name);
			wxFileConfig * metadata_config = new wxFileConfig(cfg_stream);
			// enumeration variables
			wxString str;
			long dummy;

			// first enum all entries
			metadata_config->GetFirstGroup(str, dummy);;
			bool bCont = metadata_config->GetFirstEntry(str, dummy);
			while ( bCont ) {
				m_gridMetadata->AppendRows ();
				m_gridMetadata->SetRowLabelValue (m_gridMetadata->GetNumberRows()-1, str);
				m_gridMetadata->SetCellValue (m_gridMetadata->GetNumberRows()-1, 0, metadata_config->Read(str));
				
				bCont = metadata_config->GetNextEntry(str, dummy);
			}
			wxDELETE(metadata_config);
		}
		else
		{
			wxGetApp().Log(wxT("Failed to load project metadata list ") + config_file_name);
		}
	}
}

void ConfirmDialog::OnSize( wxSizeEvent& event ) 
{
	int width = 0, height =0, col_size=0;
	m_gridMetadata->GetClientSize(&width, &height);
	col_size = width - (m_gridMetadata->GetRowLabelSize()) ;
	if(col_size>0)
	{
		m_gridMetadata->SetColSize( 0, col_size );
	}
}

void ConfirmDialog::OnLaunch( wxCommandEvent& event )
{
	// Generate exchange metadata file
	wxString exchange_filename = wxFileName::CreateTempFileName(wxT("session"));

	if( exchange_filename != wxEmptyString )
	{
		wxTextFile exchange_file(exchange_filename);
		exchange_file.Exists() ? exchange_file.Open() : exchange_file.Create() ;
		if( exchange_file.IsOpened())
		{
			int row ;
			for(row=0; row<m_gridMetadata->GetNumberRows(); row++)
			{
				exchange_file.AddLine(
					wxString::Format(
						"%s=%s",
						m_gridMetadata->GetRowLabelValue( row ),
						m_gridMetadata->GetCellValue( row, 0 )
					)
				);
			}
			exchange_file.Write();
			exchange_file.Close();
		}
	}

	Show(false);

	ProcessController::Get()->StartNewSession(exchange_filename);

	return ;
}

void ConfirmDialog::ConfirmNewSession(const wxString & equipment_id)
{
	ResetGridMetadata();
	
	// update equipment id
	m_gridMetadata->SetCellValue (1, 0, equipment_id);
	
	// retrieve booking information here
	Show(true);
}

void ConfirmDialog::ResetGridMetadata()
{
	wxString equipment_id;

	if(m_gridMetadata->GetNumberRows()>0)
	{
		equipment_id = m_gridMetadata->GetCellValue (1, 0);
		m_gridMetadata->DeleteRows (0, m_gridMetadata->GetNumberRows());
	}
	m_gridMetadata->InsertRows (0, 5) ;
	m_gridMetadata->SetRowLabelValue (0, wxT("session.datetime"));
	m_gridMetadata->SetRowLabelValue (1, wxT("session.equipment.id"));
	m_gridMetadata->SetRowLabelValue (2, wxT("session.project.id"));
	m_gridMetadata->SetRowLabelValue (3, wxT("session.project.name"));
	m_gridMetadata->SetRowLabelValue (4, wxT("session.project.leader"));

	m_gridMetadata->SetReadOnly (0, 0);
	m_gridMetadata->SetReadOnly (1, 0);
	m_gridMetadata->SetReadOnly (2, 0);
	m_gridMetadata->SetReadOnly (3, 0);
	m_gridMetadata->SetReadOnly (4, 0);
	
	m_gridMetadata->SetCellTextColour(0, 0, wxSystemSettings::GetColour( wxSYS_COLOUR_GRAYTEXT));
	m_gridMetadata->SetCellTextColour(1, 0, wxSystemSettings::GetColour( wxSYS_COLOUR_GRAYTEXT));
	m_gridMetadata->SetCellTextColour(2, 0, wxSystemSettings::GetColour( wxSYS_COLOUR_GRAYTEXT));
	m_gridMetadata->SetCellTextColour(3, 0, wxSystemSettings::GetColour( wxSYS_COLOUR_GRAYTEXT));
	m_gridMetadata->SetCellTextColour(4, 0, wxSystemSettings::GetColour( wxSYS_COLOUR_GRAYTEXT));


	m_gridMetadata->SetCellValue (0, 0, wxDateTime::Now().FormatISOCombined());
	m_gridMetadata->SetCellValue (1, 0, equipment_id);
	if(m_choiceProject->GetStringSelection() != wxEmptyString)
	{
		m_gridMetadata->SetCellValue (2, 0, m_choiceProject->GetStringSelection() );
		m_gridMetadata->SetCellValue (3, 0, wxFileConfig::Get()->Read(
					wxT("project.")
					+ m_choiceProject->GetStringSelection()
					+ wxT(".name")
				));
		m_gridMetadata->SetCellValue (4, 0, wxFileConfig::Get()->Read(
					wxT("project.")
					+ m_choiceProject->GetStringSelection()
					+ wxT(".owner")
				));
	}

}

