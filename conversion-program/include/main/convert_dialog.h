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

#ifndef INCLUDED_MAIN_CONVERT_DIALOG_H
#define INCLUDED_MAIN_CONVERT_DIALOG_H


#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/filepicker.h>
#include <wx/textctrl.h>
#include <wx/gauge.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>


///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ConvertDialog
///////////////////////////////////////////////////////////////////////////////
class ConvertDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxFilePickerCtrl* m_file_from;
		wxStaticText* m_staticText2;
		wxTextCtrl* m_file_to;
		wxGauge* m_progress;
		wxStdDialogButtonSizer* m_sdbSizer4;
		wxButton* m_sdbSizer4Apply;
		wxButton* m_sdbSizer4Cancel;
	
	public:
		
		ConvertDialog( wxWindow* parent = NULL, wxWindowID id = wxID_ANY, const wxString& title = wxT("DC20A: Conversion Program"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 330,214 ), long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP ); 
		~ConvertDialog();
		virtual void OnApply( wxCommandEvent& event ) ;
		virtual void OnCancel( wxCommandEvent& event ) ;
		virtual void OnClose( wxCloseEvent& event ) ;
};







#endif
 