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

#include <wx/wfstream.h>
#include <wx/fileconf.h>
#include <wx/msgdlg.h>  



#include "icon.xpm"

#include "main/convert_dialog.h"
#include "main/convert_thread.h"
#include "main/convertor.h"


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
	
	m_progress = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxSize( -1,-1 ), wxGA_HORIZONTAL|wxGA_SMOOTH );
	m_progress->SetValue( 0 ); 
	bSizer4->Add( m_progress, 0, wxALL|wxEXPAND, 5 );
	
	m_stage = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxSize( -1,-1 ), wxGA_HORIZONTAL|wxGA_SMOOTH );
	m_stage->SetValue( 0 ); 
	bSizer4->Add( m_stage, 0, wxALL|wxEXPAND, 5 );
	
	m_staticTips = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTips->Wrap( -1 );
	bSizer4->Add( m_staticTips, 0, wxALL|wxEXPAND, 5 );
	
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
	m_timer = new wxTimer(this) ;
	Connect( wxEVT_TIMER , wxTimerEventHandler( ConvertDialog::OnTimer ));
	Connect( wxEVT_COMMAND_CONVERTTHREAD_COMPLETED , wxThreadEventHandler( ConvertDialog::OnThreadCompleted ));
	Connect( wxEVT_COMMAND_CONVERTTHREAD_UPDATE    , wxThreadEventHandler( ConvertDialog::OnThreadUpdated ));
	
	m_thread = NULL ;
	m_isAutoConvertMode = false;
}

ConvertDialog::~ConvertDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ConvertDialog::OnClose ) );
	m_sdbSizer4Apply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConvertDialog::OnApply ), NULL, this );
	m_sdbSizer4Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConvertDialog::OnCancel ), NULL, this );
	Disconnect( wxEVT_TIMER , wxTimerEventHandler( ConvertDialog::OnTimer ));
	Disconnect( wxEVT_COMMAND_CONVERTTHREAD_COMPLETED , wxThreadEventHandler( ConvertDialog::OnThreadCompleted ));

	wxDELETE(m_timer);
}

///////////////////////////////////////////////////////////////////////////

void ConvertDialog::OnApply(wxCommandEvent& event)
{
	m_timer->Start();

	m_thread = new ConvertThread(((wxDialog *)this), m_file_from->GetFileName().GetFullPath());

	if ( m_thread->Create() != wxTHREAD_NO_ERROR )
	{
		wxDELETE(m_thread);
		m_thread = NULL;
	}
	else
	{
		if (m_thread->Run() != wxTHREAD_NO_ERROR )
		{
			wxDELETE(m_thread);
			m_thread = NULL;
		}

		m_sdbSizer4Apply->Disable();
		// after the call to wxThread::Run(), the m_pThread pointer is "unsafe":
		// at any moment the thread may cease to exist (because it completes its work).
		// To avoid dangling pointers OnThreadExit() will set m_pThread
		// to NULL when the thread dies.
	}

}

void ConvertDialog::OnCancel(wxCommandEvent& event)
{
	this->Close(true);
}

void ConvertDialog::OnClose(wxCloseEvent& event)
{
	m_timer->Stop();

	Destroy();
}

void ConvertDialog::OnTimer( wxTimerEvent& event )
{
	Convertor * convertor = Convertor::Get() ;
	m_progress->SetValue( convertor->GetProgress() );
}

void ConvertDialog::OnThreadCompleted(wxThreadEvent & event)
{
	m_progress->SetValue( 100);
	m_stage->SetValue( 100);
	m_staticTips->SetLabelText(wxT("Done"));
	m_sdbSizer4Apply->Enable();
	
	if(m_isAutoConvertMode)
	{
		Convertor * convertor = Convertor::Get() ;
		wxArrayString file_lists(convertor->GetGeneratedFiles());

		wxFileInputStream fis(m_exchange_filename.GetFullPath());
		if(fis.IsOk())
		{
			wxFileConfig fc(fis);
			long count = fc.ReadLong(wxT("files/count"), 0);
			int i;
			for(i=0; i<file_lists.GetCount() ; i++)
			{
				fc.Write(wxString::Format("files/file%d", count+i+1) ,file_lists.Item(i) ) ;
			}
			fc.Write(wxT("files/count") , file_lists.GetCount() + count ) ;

			wxFileOutputStream fs(m_exchange_filename.GetFullPath());
			fc.Save(fs) ;
			fc.Flush();
			fs.Close();
		}
		
		Destroy() ;
	}
}

void ConvertDialog::OnThreadUpdated(wxThreadEvent & event)
{
	int stage 			= event.GetInt();
	int stage_total 	= event.GetExtraLong() ;
	m_stage->SetValue( 100 * stage / stage_total);
	m_staticTips->SetLabelText(wxString::Format("Converting file %d/%d", stage+1, stage_total));
}

bool ConvertDialog::AutoConvert(wxFileName filename)
{
	m_isAutoConvertMode = true ;
	m_exchange_filename = filename ;

	m_timer->Start();
	
	wxArrayString file_lists ;
	wxFileInputStream fis(m_exchange_filename.GetFullPath());
	if(fis.IsOk())
	{
		wxFileConfig fc(fis);
		long count = fc.ReadLong(wxT("files/count"), 0);
		int i;
		for(i=1; i<=count ; i++)
		{
			wxString f = fc.Read(wxString::Format("files/file%d", i) ,wxEmptyString) ;
			if( f!=wxEmptyString )
			{
				file_lists.Add(f) ;
			}
		}
	}

	m_thread = new ConvertThread(((wxDialog *)this), file_lists);

	if ( m_thread->Create() != wxTHREAD_NO_ERROR )
	{
		wxDELETE(m_thread);
		m_thread = NULL;
	}
	else
	{
		if (m_thread->Run() != wxTHREAD_NO_ERROR )
		{
			wxDELETE(m_thread);
			m_thread = NULL;
		}

		m_file_from->Disable();
		m_sdbSizer4Apply->Disable();
		
		return true ;
		// after the call to wxThread::Run(), the m_pThread pointer is "unsafe":
		// at any moment the thread may cease to exist (because it completes its work).
		// To avoid dangling pointers OnThreadExit() will set m_pThread
		// to NULL when the thread dies.
	}
	
	return false;

}