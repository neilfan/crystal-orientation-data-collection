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

#NoEnv
#NoTrayIcon 


SetKeyDelay			50
SetControlDelay		50
BlockInput				SendAndMouse

HKL_EXECUTEABLE		:= "C:\CHANNEL5\Manager.exe"
HKL_FOLDER			:= "C:\CHANNEL5\"
HKL_MAINCLASS		:= "TfPrjManagerMain"
HKL_PROFCLASS		:= "THKLProfileForm"

inputFile = %1%
outputFile := RegExReplace(inputFile, ".*\\") . ".tmp"
if ! FileExist(inputFile)
{
	ExitApp
}

iFWinNotExist ahk_class %HKL_MAINCLASS%
{
	Run %HKL_EXECUTEABLE%, %HKL_FOLDER%, UseErrorLevel
	if ErrorLevel
	{
		Exit 0
	}
	WinWait ahk_class %HKL_PROFCLASS%
	WinActivate ahk_class %HKL_PROFCLASS%
	WinWaitActive ahk_class %HKL_PROFCLASS%
	Send {ENTER}
	WinWait ahk_class %HKL_MAINCLASS%
}


IniRead files_count, %inputFile%, files, count, 0

Loop %files_count%
{

	IniRead cpr_name, %inputFile%, files, file%A_INDEX%
	ext := SubStr(cpr_name, -2)
	StringUpper ext, ext 
	IFEqual ext,CPR
	{
		; Type in the .CPR file
		{
			WinActivate ahk_class %HKL_MAINCLASS%
			WinWaitActive ahk_class %HKL_MAINCLASS%
			Send {CTRLDOWN}o{CTRLUP}
			Send {DEL}
			; Remove the / and \\ characters
			StringReplace cpr_name, cpr_name, \\, \, 1
			StringReplace cpr_name, cpr_name, /, \, 1

			Send %cpr_name%
			Send {ENTER}
		}


		; Output as .CTF file
		{
			ctf_name := SubStr(cpr_name, 1, -3) . "ctf"
			WinActivate ahk_class %HKL_MAINCLASS%
			WinWaitActive ahk_class %HKL_MAINCLASS%
			Send {ALTDOWN}pe{ALTUP}{DOWN}{ENTER}
			Send {DEL}
			Send %ctf_name%
			Send {ENTER}
			Send {ALTDOWN}y{ALTUP} ; Overwrite existing file
			WinActivate ahk_class %HKL_MAINCLASS%
			WinWaitActive ahk_class %HKL_MAINCLASS%

			; Deal with not responsding window
			NR_temp =0 ; init
			TimeOut = 100 ; milliseconds to wait before deciding it is not responding - 100 ms seems reliable under 100% usage
			; WM_NULL =0x0000
			; SMTO_ABORTIFHUNG =0x0002
			WinGet wid, ID, ahk_class %HKL_MAINCLASS% ; retrieve the ID of a window to check
			Loop
			{
				Responding := DllCall("SendMessageTimeout", "UInt", wid, "UInt", 0x0000, "Int", 0, "Int", 0, "UInt", 0x0002, "UInt", TimeOut, "UInt *", NR_temp)
				If Responding = 1 
				{
					; 1= responding, 0 = Not Responding
					Break
				}
				Sleep, 500 ; wait half a second before trying again
			}
		}
	}

}

WinActivate ahk_class %HKL_MAINCLASS%
WinWaitActive ahk_class %HKL_MAINCLASS%
Send {ALTDOWN}{F4}{ALTUP}
