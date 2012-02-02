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

#include "main/convert_thread.h"
#include "main/convertor.h"
#include "main/app.h"

wxDEFINE_EVENT(wxEVT_COMMAND_CONVERTTHREAD_COMPLETED, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_COMMAND_CONVERTTHREAD_UPDATE,    wxThreadEvent);


ConvertThread::ConvertThread(wxDialog * handler, const wxString & input) : wxThread(wxTHREAD_DETACHED)
{
	m_input.Add(input) ;
	m_pHandler = handler;
}

ConvertThread::ConvertThread(wxDialog * handler, const wxArrayString & input_ary) : wxThread(wxTHREAD_DETACHED)
{
	m_input = input_ary ;
	m_pHandler = handler;
}

ConvertThread::~ConvertThread()
{
}


wxThread::ExitCode ConvertThread::Entry()
{
	Convertor * convertor = Convertor::Get() ;
	
	size_t i;
	for(i=0; i<m_input.GetCount(); i++)
	{
		wxThreadEvent * evt = new wxThreadEvent(wxEVT_COMMAND_CONVERTTHREAD_UPDATE) ;
		evt->SetExtraLong(m_input.GetCount());
		evt->SetInt(i);
		wxQueueEvent(m_pHandler, evt);
		
		wxArrayString array = wxStringTokenize(m_input.Item(i) , FILENAME_DELIM) ;
		if(array.GetCount()==1)
		{
			// by default upload to SESSION directory
			array.Add(wxT(""));
		}
		convertor->SetDest(array[1]);

		if( convertor->LoadDataFile(array[0]) )
		{
			convertor->ToAstar() ;
			convertor->ToHkl  () ;
			convertor->ToTsl  () ;
		}
	}

	// signal the event handler that this thread is going to be destroyed
	// NOTE: here we assume that using the m_pHandler pointer is safe,
	//       (in this case this is assured by the MyFrame destructor)

	wxQueueEvent(m_pHandler, new wxThreadEvent(wxEVT_COMMAND_CONVERTTHREAD_COMPLETED));
	return (wxThread::ExitCode)0;     // success
}

