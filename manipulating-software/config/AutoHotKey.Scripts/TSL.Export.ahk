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

SetKeyDelay			50
SetControlDelay		50
BlockInput			SendAndMouse

TSL_MAIN_WINNAME=OIM Analysis
TSL_PROJECTVIEW_WINCLASSNN=AfxControlBar803
TSL_PROJECTVIEW_WINTITLE=Project Tree
TSL_EXPORTTOOL_WINCLASSNN=SysTreeView321
TSL_EXPORT_EXECUTEABLE=C:\Program Files\TexSEM\OIM Analysis 5\bin\OimA.exe
TSL_EXPOER_FOLDER=C:\Program Files\TexSEM\OIM Analysis 5\bin
TSL_WRITING_DIALOG=Writing OIM Data

inputFile = %1%
if ! FileExist(inputFile)
{
	ExitApp
}


; Find if analysis window already open
; if already exists, create a new project
; this will dispose all old data







IniRead files_count, %inputFile%, files, count, 0

Loop %files_count%
{
	IniRead osc_name, %inputFile%, files, file%A_INDEX%
	ext := SubStr(osc_name, -2)

	StringUpper ext, ext 

	IFEqual ext,OSC
	{
		Run %TSL_EXPORT_EXECUTEABLE%, %TSL_EXPOER_FOLDER%, UseErrorLevel, PID
		if ErrorLevel
		{
			Exit 0
		}

		WinWait %TSL_MAIN_WINNAME% ahk_pid %PID%
		WinActivate %TSL_MAIN_WINNAME% ahk_pid %PID%
		WinWaitActive %TSL_MAIN_WINNAME% ahk_pid %PID%


		; Export what we need
		Send {CTRLDOWN}o{CTRLUP}
		Send {ALTDOWN}n{ALTUP} ; Do not want to save current project
		Send {DEL}

		StringReplace osc_name, osc_name, \\, \, 1
		StringReplace osc_name, osc_name, /, \, 1
		Send %osc_name%
		; a MDI window will show with the file name
		SplitPath osc_name,,,,file_name

		Send {ENTER}
		WinWait %file_name% ahk_pid %PID%
		WinActivate %TSL_MAIN_WINNAME%  ahk_pid %PID%


		ControlFocus %TSL_EXPORTTOOL_WINCLASSNN%, %TSL_MAIN_WINNAME%
		ControlClick  %TSL_EXPORTTOOL_WINCLASSNN%, %TSL_MAIN_WINNAME%

		Sleep 1000 ; wait for a while as the control has a short delay
		IfWinExist %TSL_PROJECTVIEW_WINTITLE%
		{
			; Project View is a toolbar window, no docking
			WinGetPos, x,y,w,h, %TSL_PROJECTVIEW_WINTITLE%
			CoordMode Mouse, Screen
			MouseMove %x%, %y%
			CoordMode Mouse, Relative
			ControlGetPos x,y,w,h,%TSL_EXPORTTOOL_WINCLASSNN%, %TSL_PROJECTVIEW_WINTITLE%
			MouseMove %x%, %y%,,R
		}
		else
		{
			; Project View is a docking window
			ControlGetPos x,y,w,h,%TSL_EXPORTTOOL_WINCLASSNN%, %TSL_MAIN_WINNAME%
			MouseMove %x%, %y%
		}
		
		; move the mouse to first item
		MouseMove 40, 20, ,R
		Send {CLICKRIGHT}

		ang_name := SubStr(osc_name, 1, -3) . "ang"

		Send e
		Send d
		Send {DEL}
		Send %ang_name%
		Send {ENTER}
		Send {ALTDOWN}y{ALTUP} ; Overwrite existing file

		While WinExist(TSL_WRITING_DIALOG)
		{
			Sleep 1000
		}
		
		WinActivate %TSL_MAIN_WINNAME%  ahk_pid %PID%
		WinWaitActive %TSL_MAIN_WINNAME%  ahk_pid %PID%
		Send {ALTDOWN}{F4}{ALTUP}
		Send {ALTDOWN}n{ALTUP}
		
		Sleep 1000

	}
}
