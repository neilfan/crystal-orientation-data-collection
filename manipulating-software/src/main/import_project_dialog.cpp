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

#include <wx/icon.h>
#include <wx/file.h>
#include <wx/textfile.h>
#include <wx/fileconf.h> 
#include <wx/utils.h> 


#include "main/import_project_dialog.h"
#include "main/project_updater.h"
#include "main/app.h"

#include "icon.xpm"

ImportProjectDialog::ImportProjectDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer7->AddGrowableCol( 0 );
	fgSizer7->AddGrowableRow( 3 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	fgSizer7->Add( 0, 10, 1, wxEXPAND, 5 );
	
	labelTitle = new wxStaticText( this, wxID_ANY, wxT("Import project and define metadata to be collected."), wxDefaultPosition, wxDefaultSize, 0 );
	labelTitle->Wrap( -1 );
	fgSizer7->Add( labelTitle, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer7->Add( 0, 10, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* sizerMiddle;
	sizerMiddle = new wxFlexGridSizer( 0, 1, 0, 0 );
	sizerMiddle->AddGrowableCol( 0 );
	sizerMiddle->AddGrowableRow( 4 );
	sizerMiddle->SetFlexibleDirection( wxBOTH );
	sizerMiddle->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticText* labelProject;
	labelProject = new wxStaticText( this, wxID_ANY, wxT("Research Project"), wxDefaultPosition, wxDefaultSize, 0 );
	labelProject->Wrap( -1 );
	sizerMiddle->Add( labelProject, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxFlexGridSizer* fgSizer20;
	fgSizer20 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer20->AddGrowableCol( 1 );
	fgSizer20->SetFlexibleDirection( wxBOTH );
	fgSizer20->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	fgSizer20->Add( 40, 0, 1, wxEXPAND, 5 );
	
	wxString m_choiceProjectChoices[] = { wxEmptyString };
	int m_choiceProjectNChoices = sizeof( m_choiceProjectChoices ) / sizeof( wxString );
	m_choiceProject = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceProjectNChoices, m_choiceProjectChoices, 0 );
	m_choiceProject->SetSelection( 0 );
	fgSizer20->Add( m_choiceProject, 0, wxALL|wxEXPAND, 5 );
	
	sizerMiddle->Add( fgSizer20, 1, wxEXPAND, 5 );
	
	
	sizerMiddle->Add( 0, 20, 1, wxEXPAND, 5 );
	
	wxStaticText* staticTextMetadata;
	staticTextMetadata = new wxStaticText( this, wxID_ANY, wxT("Define Metadata"), wxDefaultPosition, wxDefaultSize, 0 );
	staticTextMetadata->Wrap( -1 );
	sizerMiddle->Add( staticTextMetadata, 0, wxALIGN_TOP|wxALL, 5 );
	
	wxFlexGridSizer* fgSizer21;
	fgSizer21 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer21->AddGrowableCol( 1 );
	fgSizer21->AddGrowableRow( 0 );
	fgSizer21->SetFlexibleDirection( wxBOTH );
	fgSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	fgSizer21->Add( 40, 0, 1, wxEXPAND, 5 );
	
	m_gridMetadata = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), 0 );
	
	// Grid
	m_gridMetadata->CreateGrid( 1, 2 );
	m_gridMetadata->EnableEditing( true );
	m_gridMetadata->EnableGridLines( true );
	m_gridMetadata->EnableDragGridSize( false );
	m_gridMetadata->SetMargins( 0, 0 );
	
	// Columns
	m_gridMetadata->SetColSize( 0, 150 );
	m_gridMetadata->SetColSize( 1, 150 );
	m_gridMetadata->EnableDragColMove( false );
	m_gridMetadata->EnableDragColSize( true );
	m_gridMetadata->SetColLabelSize( 24 );
	m_gridMetadata->SetColLabelValue( 0, wxT("Name") );
	m_gridMetadata->SetColLabelValue( 1, wxT("Default Value") );
	m_gridMetadata->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	// TODO disable autoizerows due to a bug: 
	//      delete contents of a row then press enter
	//      will failure on assertion
	//m_gridMetadata->AutoSizeRows(true);
	m_gridMetadata->EnableDragRowSize( false );
	m_gridMetadata->SetRowLabelSize( 0 );
	m_gridMetadata->SetRowLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gridMetadata->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	
	fgSizer21->Add( m_gridMetadata, 0, wxALL|wxEXPAND, 5 );
	
	sizerMiddle->Add( fgSizer21, 1, wxEXPAND, 5 );
	
	fgSizer7->Add( sizerMiddle, 1, wxEXPAND, 5 );
	
	
	fgSizer7->Add( 0, 10, 1, wxEXPAND, 5 );
	
	m_cmdButtons = new wxStdDialogButtonSizer();
	m_cmdButtonsApply = new wxButton( this, wxID_APPLY );
	m_cmdButtons->AddButton( m_cmdButtonsApply );
	
	m_cmdButtonsCancel = new wxButton( this, wxID_CANCEL );
	m_cmdButtons->AddButton( m_cmdButtonsCancel );
	m_cmdButtons->Realize();
	fgSizer7->Add( m_cmdButtons, 1, wxALL|wxEXPAND, 5 );
	
	
	fgSizer7->Add( 0, 10, 1, wxEXPAND, 5 );
	
	this->SetSizer( fgSizer7 );
	this->Layout();
	
	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( ImportProjectDialog::OnSize ) );
	m_gridMetadata->Connect( wxEVT_GRID_CELL_CHANGE, wxGridEventHandler( ImportProjectDialog::OnGridCellChange ), NULL, this );
	m_cmdButtonsApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImportProjectDialog::OnApplyButtonClick ), NULL, this );


	/**
	 * Extra code not imported from wxFormBuilder
	 * Make sure not overwritten by update on form builder
	 */
	// set up icon
	this->SetIcon(icon_xpm);




	LoadProjects();
}

ImportProjectDialog::~ImportProjectDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( ImportProjectDialog::OnSize ) );
	m_gridMetadata->Disconnect( wxEVT_GRID_CELL_CHANGE, wxGridEventHandler( ImportProjectDialog::OnGridCellChange ), NULL, this );
	m_cmdButtonsApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImportProjectDialog::OnApplyButtonClick ), NULL, this );

}

void ImportProjectDialog::LoadProjects()
{
	wxFileName f(PROJECT_LIST_FILENAME) ;

	if( ! f.FileExists())
	{
		ProjectUpdater::Get()->Update();
	}
	else
	{
		wxString str ;

        wxArrayString projects;

		wxTextFile f_pl(f.GetFullPath()) ;
		f_pl.Open() ;

        for ( str = f_pl.GetFirstLine(); ! f_pl.Eof(); str = f_pl.GetNextLine() )
        {
			// omit the pids already configured here
			wxFileName fn("projects", str, "ini") ;
			if( ! fn.FileExists() )
			{
                projects.Add(str);
			}
        }
        projects.Sort();
        for(size_t i=0; i<projects.GetCount(); i++)
        {
            m_choiceProject->Append( projects.Item(i) ) ;
        }

		f_pl.Close() ;
	}
}


void ImportProjectDialog::OnSize( wxSizeEvent& event )
{
	int width = 0, height =0, col_size=0;
	m_gridMetadata->GetClientSize(&width, &height);
	col_size = width - (m_gridMetadata->GetColSize(0)) ;

	if(col_size>0)
	{
		m_gridMetadata->SetColSize( 1, col_size );
	}

    event.Skip();
}

void ImportProjectDialog::OnGridCellChange( wxGridEvent& event )
{
	int row = event.GetRow() ;
	if( row == m_gridMetadata->GetNumberRows() -1 )
	{
		// if current line is the last line
		// add a new line if user enters something

		if(
			m_gridMetadata->GetCellValue( row , 0 ).Length() > 0 ||
			m_gridMetadata->GetCellValue( row , 1 ).Length() > 0
		)
		{
			m_gridMetadata->AppendRows();
		}
	}
	else
	{
		// if current line is not last line
		// remove if user delete all data in this row 
		if(
			m_gridMetadata->GetCellValue( row , 0 ).Length() == 0 &&
			m_gridMetadata->GetCellValue( row , 1 ).Length() == 0
		)
		{
			m_gridMetadata->DeleteRows( row, 1, false );
		}
	}

	event.Skip();
}

void ImportProjectDialog::OnApplyButtonClick( wxCommandEvent& event )
{
	// validate here


	wxArrayString errs;

	// 1. if a project selected?
	if(m_choiceProject->GetSelection() == 0)
	{
		errs.Add("Please select a project to import.");
	}


    /**
	// 2. provides project owner ?
	if(m_gridMetadata->GetCellValue(0,1).Length() == 0)
	{
		errs.Add("Please provide the contact of project owner.");
	}
    **/

	// 3. any metadata without a name?
	for(int i=0; i<m_gridMetadata->GetNumberRows(); i++)
	{
		if(
			m_gridMetadata->GetCellValue(i, 0).Length() == 0 &&
			m_gridMetadata->GetCellValue(i, 1).Length() > 0
		)
		{
			errs.Add("Metadata requires a name");
			break ;
		}
	}

	// 4. any metadata without a "=" in name?
	for(int i=0; i<m_gridMetadata->GetNumberRows(); i++)
	{
		if( m_gridMetadata->GetCellValue(i, 0).Contains("=") )
		{
			errs.Add("Metadata name can not contain a equal symbol (=)");
			break ;
		}
	}


	// Check if any error found
	if( errs.GetCount() > 0 )
	{
		wxMessageBox(
			wxJoin( errs, '\n' ),
			"Error",
			wxOK | wxICON_ERROR
		);
		return ;
	}


	// Seems all good here, let's create a temp ini file
	wxFileName file_name("projects", m_choiceProject->GetStringSelection() , "ini") ;
	wxTempFile temp_file(file_name.GetFullPath());
	if(temp_file.IsOpened())
	{
		temp_file.Write("[project]");
		temp_file.Write(wxTextFile::GetEOL());

        /**
		for(int i=0; i<1; i++)
		{
			temp_file.Write(
				wxString::Format(
					"%s=%s",
					m_gridMetadata->GetCellValue(i, 0),
					m_gridMetadata->GetCellValue(i, 1)
			));
			temp_file.Write(wxTextFile::GetEOL());
		}
        **/

		temp_file.Write("[metadata]");
		temp_file.Write(wxTextFile::GetEOL());

		for(int i=0; i<m_gridMetadata->GetNumberRows(); i++)
		{
			wxString metadata_name = m_gridMetadata->GetCellValue(i, 0) ;
			if( metadata_name.Length() > 0 )
			{
				temp_file.Write(
					wxString::Format(
						"%s=%s",
						m_gridMetadata->GetCellValue(i, 0),
						m_gridMetadata->GetCellValue(i, 1)
						)
				);
				temp_file.Write(wxTextFile::GetEOL());
			}
		}

		bool is_committed = temp_file.Commit() ;
		if( is_committed )
		{
			wxGetApp().Log(
				wxString::Format(
					"Profile created for project %s",
					m_choiceProject->GetStringSelection()
			));

		}
		else
		{
			wxGetApp().Log(
				wxString::Format(
					"Failed to create profile for project %s",
					m_choiceProject->GetStringSelection()
			));
		}

		if( is_committed )
		{
			// transfer file to remote server
            TransferProjectProfile();

            // And close this dialog
			Close();
		}
	}

}

void ImportProjectDialog::TransferProjectProfile()
{
    wxString projectId = m_choiceProject->GetStringSelection() ;
	wxFileName local("projects", projectId , "ini") ;

    wxString remoteDir = wxFileConfig::Get()->Read(wxT("sys.network.smb.storage")) ;
    remoteDir.Replace(wxT("%PID%"), projectId) ;

	wxFileName dest(remoteDir , "PROJECT.ini") ;

    wxString cmd_line = ProjectUpdater::Get()->GetCommandLine(wxString::Format(
        "put %s %s",
        local.GetFullPath(),
        dest.GetFullPath()
    )) ;

    wxExecute(cmd_line,  wxEXEC_ASYNC | wxEXEC_NODISABLE | wxEXEC_NOEVENTS | wxEXEC_HIDE_CONSOLE );
}
