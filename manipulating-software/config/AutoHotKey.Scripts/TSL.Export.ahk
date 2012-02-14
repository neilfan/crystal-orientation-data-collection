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

TSL_MAIN_WINNAME=ahk_class Afx:00400000:8:00010011:00000000:00030693
TSL_PROJECTVIEW_WINCLASSNN=AfxControlBar803
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
IfWinExists %TSL_MAIN_WINNAME%
{
	WinActivate %TSL_MAIN_WINNAME%
	Send {CTRLDOWN}n{CTRLUP}
	; it will ask save project or not
	Send {ALTDOWN}n{ALTUP}
}
else
{
	; if not started yet, run it
	Run %TSL_EXPORT_EXECUTEABLE%, %TSL_EXPOER_FOLDER%, UseErrorLevel
	if ErrorLevel
	{
		Exit 0
	}
	WinWait %TSL_MAIN_WINNAME%
	WinActivate %TSL_MAIN_WINNAME%
	WinWaitActive %TSL_MAIN_WINNAME%
}


isProjectViewHidden = 1
WinGet controlList, ControlList, %TSL_MAIN_WINNAME%

Loop, Parse, controlList, `n
{
	IfInString  A_LoopField, %TSL_PROJECTVIEW_WINCLASSNN%
	{
		isProjectViewHidden = 0
		break
	}
}

; Project View is not shown
If isProjectViewHidden
{
	WinActivate %TSL_MAIN_WINNAME%
	Send {CTRLDOWN}t{CTRLUP}
}



IniRead files_count, %inputFile%, files, count, 0

Loop %files_count%
{
	IniRead osc_name, %inputFile%, files, file%A_INDEX%
	ext := SubStr(osc_name, -2)
	StringUpper ext, ext 
	IFEqual ext,OSC
	{
		; Export what we need
		WinActivate %TSL_MAIN_WINNAME%
		WinWaitActive %TSL_MAIN_WINNAME%
		Send {CTRLDOWN}o{CTRLUP}
		Send {ALTDOWN}n{ALTUP} ; Do not want to save current project
		Send {DEL}

		StringReplace osc_name, osc_name, \\, \, 1
		StringReplace osc_name, osc_name, /, \, 1

		Send %osc_name%
		Send {ENTER}


		WinActivate %TSL_MAIN_WINNAME%
		ControlClick  %TSL_EXPORTTOOL_WINCLASSNN%, %TSL_MAIN_WINNAME%

		ang_name := SubStr(cpr_name, 1, -3) . "ang"

		Send {Home}{Down}{AppsKey}ed
		Send {DEL}
		Send %ang_name%
		Send {ENTER}
		Send {ALTDOWN}y{ALTUP} ; Overwrite existing file
		
		While WinExists(TSL_WRITING_DIALOG)
		{
			Sleep 1000
		}
	}
}
