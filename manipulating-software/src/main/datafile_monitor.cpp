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

#include "main/datafile_monitor.h"
#include "main/process_controller.h"
#include "main/app.h"

DataFileMonitor * DataFileMonitor::m_pInstance = NULL;

DataFileMonitor * DataFileMonitor::Get()
{
	if( ! m_pInstance)
	{
		m_pInstance = new DataFileMonitor;
	}
	return m_pInstance ;
}

DataFileMonitor :: DataFileMonitor()
{
	m_isMorniting = false;
	m_arrayExts = new wxArrayString() ;
}

DataFileMonitor :: ~DataFileMonitor()
{
	wxDELETE(m_arrayExts);
}

/**
 * Process the change on filesystem which is just reported.
 * Need to check the extension of file. (lower case stored in the m_arrayExts)
 */
void DataFileMonitor :: OnFileSystemEvent(wxFileSystemWatcherEvent& event)
{
	if( ! event.IsError())
	{
		wxFileName filename = event.GetNewPath() ;
		wxString ext = filename.GetExt().Lower();

		if( m_arrayExts->GetCount() == 0)
		{
			// TODO: Report to process controller of this file creation
			ProcessController::Get()->OnNewDataFileFound( filename.GetFullPath() ) ;
			return;
		}

		unsigned int i ;
		for( i=0 ;i<m_arrayExts->GetCount() ; i++)
		{
			if( m_arrayExts->Item(i) == ext)
			{
				// TODO: Report to process controller of this file creation
				ProcessController::Get()->OnNewDataFileFound( filename.GetFullPath() ) ;
				break;
			}
		}
	}
}

/**
 * Start the monitoring.
 */
void DataFileMonitor :: Start()
{
	if( ! m_isMorniting )
	{
		wxGetApp().Log(wxT("DataFileMonitor Started") );

		size_t i ;
		for( i=0 ;i<m_arrayExts->GetCount() ; i++)
		{
			wxGetApp().Log(wxT("  EXT:  ") + m_arrayExts->Item(i) );
		}
		
		wxArrayString paths ;
		GetWatchedPaths(&paths) ;
		for( i=0 ;i<paths.GetCount() ; i++)
		{
			wxGetApp().Log(wxT("  PATH: ") + paths.Item(i) );
		}

		Connect(wxEVT_FSWATCHER,
				wxFileSystemWatcherEventHandler(DataFileMonitor::OnFileSystemEvent));
		m_isMorniting = true ;
	}
}

/**
 * Stop the monitoring.
 * No change with the current monitoring settings
 */
void DataFileMonitor :: Stop()
{
	if( m_isMorniting )
	{
		wxGetApp().Log(wxT("DataFileMonitor Stopped"));
		Disconnect(wxEVT_FSWATCHER,
			wxFileSystemWatcherEventHandler(DataFileMonitor::OnFileSystemEvent));
		m_isMorniting = false ;
	}
}

/**
 * Add a new file extension for monitoring
 */
void DataFileMonitor :: AddExtension(const wxString & ext)
{
	wxString ext_lower = ext ;
	ext_lower = ext_lower.Trim().Lower() ;

	if( ext_lower != wxEmptyString )
	{
		
		unsigned int i ;
		for( i=0 ;i<m_arrayExts->GetCount() ; i++)
		{
			if( m_arrayExts->Item(i) == ext_lower)
			{
				return ;
			}
		}
		m_arrayExts->Add(ext_lower);
	}
}


/**
 * check if the monitor ON or OFF
 */
bool DataFileMonitor :: IsMoniotring()
{
	return m_isMorniting;
}


/**
 * Reset the data file monitor
 * Stop all monitoring and disconnect the event
 */
void DataFileMonitor :: Reset()
{
	wxDELETE(m_pInstance);
}
