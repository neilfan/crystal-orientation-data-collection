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
#include <wx/regex.h>
#include <wx/tokenzr.h>
#include <math.h>
#include <float.h>

#include "main/convertor.h"
#include "main/app.h"
#include "main/datarow.h"
#include "main/datarow_ang.h"
#include "main/datarow_astar.h"
#include "main/datarow_hkl.h"
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

void Convertor::SetDest(const wxString & dest)
{
	m_dest = dest ;
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
		// Their first line is different
		wxString line = m_textfile->GetFirstLine();
		if(line.StartsWith("# File created from ACOM RES results"))
		{
			m_format = Convertor::FORMAT_ASTAR;
		}

		if(line.StartsWith("# TEM_PIXperUM"))
		{
			m_format = Convertor::FORMAT_TSL;
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
		(current_format==Convertor::FORMAT_TSL && format==Convertor::FORMAT_ASTAR) ||
		(current_format==Convertor::FORMAT_ASTAR && format==Convertor::FORMAT_TSL)
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
			case Convertor::FORMAT_UNKNOW:
				return false;
				break;
			case Convertor::FORMAT_ASTAR:
				filename->SetName( filename->GetName() + wxT("-Converted") ) ;
				filename->SetExt(FILENAMEEXT_ANG);
				break;
			case Convertor::FORMAT_HKL:
				filename->SetName( filename->GetName() + wxT("-Converted") ) ;
				filename->SetExt(FILENAMEEXT_CTF);
				break;
			case Convertor::FORMAT_TSL:
				filename->SetName( filename->GetName() + wxT("-Converted") ) ;
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
	switch(current_format)
	{
		case Convertor::FORMAT_UNKNOW:
			break;
		case Convertor::FORMAT_ASTAR:
			outRow = new DataRowASTAR() ;
			break;
		case Convertor::FORMAT_HKL:
			outRow = new DataRowHKL() ;
			break;
		case Convertor::FORMAT_TSL:
			outRow = new DataRowTSL() ;
			break;
	}
	/**
	 * Process Header Section
	 * Phases[]
	 *     0-5 : 6 digits
	 *     6: material name
	 *     7: Luna group
	 */
	if(
		current_format==Convertor::FORMAT_ASTAR ||
		current_format==Convertor::FORMAT_TSL
	)
	{
		wxString line;
		int phase_current = 0;
		int phase_max = 0 ;
		wxArrayString phases  ;
		wxArrayString phase_output  ;

		phases.Empty() ;
		for(int i=0; i<8; i++)
		{
			phases.Add(wxEmptyString);
		}
		
		int xcells = 0 ;
		int ycells = 0 ;
		float xstep = 0.0 ;
		float ystep = 0.0 ;

		float xmin=FLT_MAX_EXP ;
		float xmax=FLT_MIN_EXP;
		float ymin=FLT_MAX_EXP ;
		float ymax=FLT_MIN_EXP;
		float xprev=FLT_MIN_EXP;
		float yprev=FLT_MIN_EXP;

		for(line=m_textfile->GetFirstLine() ; !m_textfile->Eof() ; line=m_textfile->GetNextLine() )
		{
			outRow->Import(line) ;
			if(outRow->IsComment())
			{
				{
					wxRegEx reg("# Phase[ ]+([0-9]+)");
					if( reg.Matches(line) )
					{
						for(int i=0; i<8; i++)
						{
							phases[i] = wxEmptyString ;
						}

						phase_current = wxAtoi(reg.GetMatch(line, 1));
						if(phase_max < phase_current)
						{
							phase_max = phase_current ;
						}
						continue;
					}
				}

				{
					wxRegEx reg("# MaterialName[ ]+(.+)");
					if( reg.Matches(line) )
					{
						wxString material = reg.GetMatch(line, 1) ;
						phases[6] = material ;
						continue;
					}
				}

				{
					wxRegEx reg("# Symmetry[ ]+([0-9]+)");
					if( reg.Matches(line) )
					{
						int group = wxAtoi(reg.GetMatch(line, 1)) ;
						if(group==6 || group==62)
						{
							phases[7] = wxT("9") ;
						}
						else if(group==43)
						{
							phases[7] = wxT("11") ;
						}
						continue;
					}
				}

				{
					wxRegEx reg("# XSTEP:[ ]+(.+)");
					if( reg.Matches(line) )
					{
						xstep = wxAtof(reg.GetMatch(line, 1)) ;
						continue;
					}
				}

				{
					wxRegEx reg("# YSTEP:[ ]+(.+)");
					if( reg.Matches(line) )
					{
						ystep = wxAtof(reg.GetMatch(line, 1)) ;
						continue;
					}
				}

				{
					wxRegEx reg("# NROWS:[ ]+(.+)");
					if( reg.Matches(line) )
					{
						ycells = wxAtoi(reg.GetMatch(line, 1)) ;
						continue;
					}
				}

				{
					wxRegEx reg("# NCOLS_[A-Z]+:[ ]+(.+)");
					if( reg.Matches(line) )
					{
						xcells = wxAtoi(reg.GetMatch(line, 1)) ;
						continue;
					}
				}

				{
					wxRegEx reg("# LatticeConstants[ ]+(.+)");
					if( reg.Matches(line) )
					{
						wxString digit_str = reg.GetMatch(line, 1) ;
						wxArrayString digits = wxStringTokenize(digit_str, wxT(" ")) ;
						if(digits.GetCount()==6)
						{
							phases[0] = digits[0] ;
							phases[1] = digits[1] ;
							phases[2] = digits[2] ;
							phases[3] = digits[3] ;
							phases[4] = digits[4] ;
							phases[5] = digits[5] ;
						}
						continue;
					}
				}

				{
					wxRegEx reg("# NumberFamilies.*");
					if( reg.Matches(line) )
					{
						// this is end of a phase
						// at least for the data we focus on
						phase_output.Add(
							wxString::Format(
								wxT("%f;%f;%f	%d;%d;%d	%s	%d				"),
								wxAtof(phases[0]),
								wxAtof(phases[1]),
								wxAtof(phases[2]),
								wxAtoi(phases[3]),
								wxAtoi(phases[4]),
								wxAtoi(phases[5]),
								phases[6],
								wxAtoi(phases[7])
							)
						);
						continue;
					}
				}

			}
			else
			{
				if(
					(xcells==0 || ycells==0 || xstep==0.0 || ystep==0.0) &&
					current_format==Convertor::FORMAT_ASTAR
				)
				{
					wxArrayString values = outRow->ToArrayString();
					float x=wxAtof(values[3]);
					float y=wxAtof(values[4]);

					if(xstep==0.0 && xprev!=FLT_MIN_EXP && xprev!=x)
					{
						xstep = fabs(x-xprev) ;
					}
					else
					{
						xprev=x;
					}

					if(ystep==0.0 && yprev!=FLT_MIN_EXP && yprev!=y)
					{
						ystep = fabs(y-yprev) ;
					}
					else
					{
						yprev=y;
					}


					if(x < xmin)
					{
						xmin=x ;
					}
					if(x > xmax)
					{
						xmax=x ;
					}

					if(y < ymin)
					{
						ymin=y ;
					}
					if(y > ymax)
					{
						ymax=y ;
					}


				}
			}
		}
		if(xcells==0.0 || ycells==0.0)
		{ 
			xcells = (xmax-xmin)/xstep  +1 ;
			ycells = (ymax-ymin)/ystep  +1 ;
		}

		/**
		 * Output Headers
		 */
		out.AddLine(wxT("Channel Text File"));
		out.AddLine(wxT("Prj	"));
		out.AddLine(wxT("Author	[Unknown]"));
		out.AddLine(wxT("JobMode	Grid"));
		out.AddLine(wxString::Format(wxT("XCells	%d"), xcells));
		out.AddLine(wxString::Format(wxT("YCells	%d"), ycells));
		out.AddLine(wxString::Format(wxT("XStep	%f"), xstep));
		out.AddLine(wxString::Format(wxT("YStep	%f"), ystep));
		out.AddLine(wxT("AcqE1	0"));
		out.AddLine(wxT("AcqE2	0"));
		out.AddLine(wxT("AcqE3	0"));
		out.AddLine(wxT("Euler angles refer to Sample Coordinate system (CS0)!	Mag		Coverage		Device		KV		TiltAngle		TiltAxis	"));
		out.AddLine(wxString::Format(wxT("Phases	%d"), phase_max));
		for(size_t i=0; i<phase_output.GetCount();i++)
		{
			out.AddLine(phase_output[i]);
		}
		out.AddLine(wxT("Phase	X	Y	Bands	Error	Euler1	Euler2	Euler3	MAD	BC	BS"));

	}
	


	/**
	 * Process Data Section
	 */
	wxString line;

	for(line=m_textfile->GetFirstLine() ; !m_textfile->Eof() ; line=m_textfile->GetNextLine() )
	{
		m_progress = 100 * m_textfile->GetCurrentLine()  /  m_textfile->GetLineCount() ;

		outRow->Import(line) ;
		if(!outRow->IsComment())
		{
			wxString newLine ;
			switch(format)
			{
				case Convertor::FORMAT_UNKNOW:
					break;
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

	m_generated_files.Add(filename->GetFullPath() + FILENAME_DELIM + m_dest);

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
