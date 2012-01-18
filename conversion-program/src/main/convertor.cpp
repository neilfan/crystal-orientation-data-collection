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
#include <wx/filename.h>
#include <wx/arrstr.h>
#include <wx/msgdlg.h>



#include "main/convertor.h"
#include "main/app.h"
#include "main/datarow.h"
#include "main/datarow_ang.h"
#include "main/datarow_astar.h"
//#include "main/datarow_hkl.h"
#include "main/datarow_tsl.h"

Convertor * Convertor::m_pInstance = NULL;

Convertor * Convertor::Get()
{
	if( ! m_pInstance)
	{
		m_pInstance = new Convertor;
	}
	return m_pInstance ;
}

Convertor :: ~Convertor()
{
	wxDELETE(m_textfile) ;
}

Convertor :: Convertor()
{
	m_progress = 0;
}

bool Convertor::LoadDataFile(const wxString & file)
{
	m_format = Convertor::FORMAT_UNKNOW ;

	m_textfile = new wxTextFile(file) ;
	m_textfile->Open() ;
	
	if( ! m_textfile->IsOpened() )
	{
		return false;
	}

	DataRow::SetCrystalSystemType(None);

	if(	DetermineFormat() == Convertor::FORMAT_TSL ||
		DetermineFormat() == Convertor::FORMAT_ASTAR )
	{
		// find the crystal system here
		wxString line ;
		wxString symmetry ;
		for(line=m_textfile->GetFirstLine() ; ! m_textfile->Eof() ; line=m_textfile->GetNextLine() )
		{
			DataRowANG row ;
			row.Import(line) ;
			if(row.IsComment())
			{
				if(line.StartsWith(wxT("# Symmetry"), &symmetry))
				{
					int i = wxAtoi( symmetry.Trim(false).Trim() );
					if(i==43)
					{
						DataRow::SetCrystalSystemType(Cubic);
					}
					if(i==6 || i==62)
					{
						DataRow::SetCrystalSystemType(Hexagonal);
					}
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
	
	if(DataRow::GetCrystalSystemType() == None)
	{
		return false;
	}

	if(DetermineFormat() == Convertor::FORMAT_TSL)
	{
		// load min/max IQ here
		wxString line ;
		float min=0;
		float max=0;
		bool inited = false;
		for(line=m_textfile->GetFirstLine() ; ! m_textfile->Eof() ; line=m_textfile->GetNextLine() )
		{
			DataRowTSL row ;
			row.Import(line) ;
			if(row.IsComment())
			{
				continue ;
			}
			wxArrayString as = row.ToArrayString() ;
			float iq = wxAtof(as[5]) ;
			if( !inited || min>iq)
			{
				min=iq ;
			}
			if( !inited || max<iq)
			{
				max=iq ;
			}
			inited = true;
		}
		DataRowTSL::SetIQRange(min, max);
	}

	return true ;
}

Convertor::Format Convertor::DetermineFormat()
{
	if( ! m_textfile->IsOpened())
	{
		m_format = Convertor::FORMAT_UNKNOW ;
	}
	
	if(m_format != Convertor::FORMAT_UNKNOW )
	{
		return m_format ;
	}

	wxFileName filename( m_textfile->GetName() ) ;
	
	if( filename.GetExt().Lower() == FILENAMEEXT_CTF )
	{
		m_format = Convertor::FORMAT_HKL ;
	}
	else if( filename.GetExt().Lower() == FILENAMEEXT_ANG )
	{
		// there are two type of .ang format
		// ASTAR has 9 columns, while TSL gets 10
		wxString line ;
		for(line=m_textfile->GetFirstLine() ; ! m_textfile->Eof() ; line=m_textfile->GetNextLine() )
		{
			DataRowANG rowAng ;
			rowAng.Import(line) ;
			if(rowAng.IsComment())
			{
				continue ;
			}
			
			int columnLength = rowAng.ToArrayString().GetCount() ;
			
			if(columnLength==COLUMNLENGTH_ASTAR)
			{
				m_format = Convertor::FORMAT_ASTAR;
				break;
			}
			else if(columnLength==COLUMNLENGTH_TSL)
			{
				m_format = Convertor::FORMAT_TSL ;
				break;
			}
		}
	}

	return m_format ;

}
bool Convertor::ToAstar(const wxString & output)
{
	return Convert(Convertor::FORMAT_ASTAR, output);
}

bool Convertor::ToHkl  (const wxString & output)
{
	return Convert(Convertor::FORMAT_HKL, output);
}

bool Convertor::ToTsl  (const wxString & output)
{
	return Convert(Convertor::FORMAT_TSL, output);
}


bool Convertor::Convert(Convertor::Format format, const wxString & output)
{
	m_progress = 0;

	Convertor::Format current_format = DetermineFormat() ;

	if(
		current_format==format ||
		current_format==Convertor::FORMAT_UNKNOW ||
		format==Convertor::FORMAT_UNKNOW
	)
	{
		return false ;
	}

	if( ! m_textfile->IsOpened())
	{
		return false ;
	}

	// build up the output file name

	wxFileName * filename ;

	if(output != wxEmptyString)
	{
		filename = new wxFileName(output) ;
	}
	else
	{
		filename = new wxFileName( m_textfile->GetName() ) ;
	
		switch(format)
		{
			case Convertor::FORMAT_ASTAR:
				filename->SetName( filename->GetName() + wxT("-ASTAR") ) ;
				filename->SetExt(FILENAMEEXT_ANG);
				break;
			case Convertor::FORMAT_HKL:
				filename->SetName( filename->GetName() + wxT("-HKL") ) ;
				filename->SetExt(FILENAMEEXT_CTF);
				break;
			case Convertor::FORMAT_TSL:
				filename->SetName( filename->GetName() + wxT("-TSL") ) ;
				filename->SetExt(FILENAMEEXT_ANG);
				break;
		}
	}

	wxTextFile out(filename->GetFullPath());
	if(out.Exists())
	{
		out.Open() ;
	}
	else
	{
		out.Create();
	}
	
	DataRow * outRow ;
	switch(format)
	{
		case Convertor::FORMAT_ASTAR:
			outRow = new DataRowASTAR() ;
			break;
		case Convertor::FORMAT_HKL:
			outRow = new DataRowASTAR() ;
			break;
		case Convertor::FORMAT_TSL:
			outRow = new DataRowASTAR() ;
			break;
	}

	wxString line;
	for(line=m_textfile->GetFirstLine() ; !m_textfile->Eof() ; line=m_textfile->GetNextLine() )
	{
		m_progress = 100 * m_textfile->GetCurrentLine()  /  m_textfile->GetLineCount() ;

		outRow->Import(line) ;
		if(outRow->IsComment())
		{
			out.AddLine(line) ;
		}
		else
		{
			wxString newLine ;
			switch(format)
			{
				case Convertor::FORMAT_ASTAR:
					newLine = outRow->ToASTAR() ;
					break;
				case Convertor::FORMAT_HKL:
					newLine = outRow->ToHKL() ;
					break;
				case Convertor::FORMAT_TSL:
					newLine = outRow->ToTSL() ;
					break;
			}
			out.AddLine(newLine) ;
		}
	}
	out.Write();
	out.Close();

	m_generated_files.Add(filename->GetFullPath());

	wxDELETE(filename);
	wxDELETE(outRow);

	return true ;
}

int Convertor::GetProgress()
{
	return m_progress ;
}

wxArrayString Convertor::GetGeneratedFiles()
{
	return m_generated_files ;
}
