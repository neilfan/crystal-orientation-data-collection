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

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 30 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_MAIN_NOTIFY_DIALOG_H
#define INCLUDED_MAIN_NOTIFY_DIALOG_H

#define NOTIFY_TEXT wxT("Your session is completed. \n\nAll data files acquired during this session are stored on local driver and placed in a queue for transfer to network storage. Please check session configuration file <b>%s</b> for list of data files.")

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
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
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class NotifyDialog
/// Present a notify window when session completed
///////////////////////////////////////////////////////////////////////////////
class NotifyDialog : public wxDialog 
{
private:

protected:
	wxStaticBitmap* m_icon;
	wxStaticText* m_labelNotification;
	wxButton* m_btnClose;
	
	// Virtual event handlers, overide them in your derived class
	virtual void OnClick( wxMouseEvent& event ) ;
	NotifyDialog( wxWindow* parent=NULL, wxWindowID id = wxID_ANY, const wxString& title = wxT("DC20A: Manipulating Software"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 330,194 ), long style = wxDEFAULT_DIALOG_STYLE|wxDIALOG_NO_PARENT|wxSTAY_ON_TOP ); 
	static NotifyDialog * m_pInstance ;
	

public:
	static NotifyDialog * Get();
	void Notify(const wxString & session_file) ;
	~NotifyDialog();

};

#endif
