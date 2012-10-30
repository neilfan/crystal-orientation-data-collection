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

#ifndef INCLUDED_MAIN_IMPORT_PROJECT_DIALOG_H
#define INCLUDED_MAIN_IMPORT_PROJECT_DIALOG_H

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/grid.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#include "main/define.h"

///////////////////////////////////////////////////////////////////////////////
/// Class ImportProjectDialog
///////////////////////////////////////////////////////////////////////////////
class ImportProjectDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText  * labelTitle;
		wxChoice      * m_choiceProject;
		wxGrid        * m_gridMetadata;
		wxButton      * m_cmdButtonsApply;
		wxButton      * m_cmdButtonsCancel;
		wxStdDialogButtonSizer* m_cmdButtons;


		void LoadProjects();
		void OnGridCellChange( wxGridEvent& event ) ;
		void OnApplyButtonClick( wxCommandEvent& event ) ;
		void OnSize( wxSizeEvent& event );
		void TransferProjectProfile();
        
		

		
	public:
		
		ImportProjectDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Import Project"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER ); 
		~ImportProjectDialog();
	
};

#endif //INCLUDED_MAIN_IMPORT_PROJECT_DIALOG_H

