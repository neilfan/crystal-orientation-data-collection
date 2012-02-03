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

HKL_WinClass	:= "TfFlamencoMain"
HKL_DONECLASS	:= "TdlgJobsDone"
HKL_EXECUTEABLE	:= "C:\CHANNEL5\Flamenco.exe"
HKL_FOLDER		:= "C:\CHANNEL5\"



Run %HKL_EXECUTEABLE%, %HKL_FOLDER%, UseErrorLevel, PID

if ErrorLevel
{
	Exit, 0
}

WinWait ahk_class %HKL_WinClass%

LOOP
{
	IfWinNotExist ahk_pid %PID%
	{
		Exit 0
	}
	IfWinExist ahk_class %HKL_DONECLASS%
	{
		WinActivate ahk_class %HKL_DONECLASS%
		WinWaitActive ahk_class %HKL_DONECLASS%
		Send, {ALTDOWN}c{ALTUP}
		WinWaitClose ahk_class %HKL_DONECLASS%
		WinActivate ahk_class %HKL_WinClass%
		WinWaitActive ahk_class %HKL_WinClass%
		Send, {ALTDOWN}{F4}{ALTUP}
		WinWaitClose ahk_class %HKL_WinClass%
		Exit 0
	}	
}

