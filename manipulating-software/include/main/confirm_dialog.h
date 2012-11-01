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

#ifndef INCLUDE_CONFIRM_DIALOG_H
#define INCLUDE_CONFIRM_DIALOG_H

#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/string.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/grid.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/hyperlink.h>


/**
 * The confirm dialog shown when a request
 * of new session is received
 *
 * Identify project and tech metadata here.
 * This is the only user interface requires
 * interaction.
 */
class ConfirmDialog : public wxDialog 
{
	private:
	
	protected:
		wxFlexGridSizer       * m_sizerMain;
		wxPanel               * m_panelBackground;
		wxChoice              * m_choiceProject;
		wxStaticText          * m_labelTips;
		wxHyperlinkCtrl       * m_linkProjNotFound;
		wxGrid                * m_gridMetadata;
		wxButton              * m_buttonCancel;
		wxButton              * m_buttonLaunch;
	
	public:
		ConfirmDialog(
                wxWindow* parent=NULL,
                wxWindowID id = wxID_ANY,
                const wxString& title = wxT("Starting a new research session"),
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxSize( -1,-1 ),
                long style = wxDEFAULT_DIALOG_STYLE|wxDIALOG_NO_PARENT|wxMAXIMIZE_BOX |
                             wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSTAY_ON_TOP
             ); 
		~ConfirmDialog();
		void OnProjectChoice( wxCommandEvent& event );
		void OnSize( wxSizeEvent& event ) ;
		void OnLaunch( wxCommandEvent& event );
		void OnProjectNotFoundClicked( wxHyperlinkEvent& event );
		void ConfirmNewSession(const wxString & equipment_id);
		void ResetGridMetadata();
        void UpdateProjectList();

};

#endif
