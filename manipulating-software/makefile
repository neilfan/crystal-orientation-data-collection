#
# This file is part of crystal-orientation-data-collection.
# 
# crystal-orientation-data-collection is free software: you can
# redistribute it and/or modify it under the terms of the GNU
# General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your
# option) any later version.
# 
# crystal-orientation-data-collection is distributed in the
# hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
# Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with crystal-orientation-data-collection. If not, see
# <http://www.gnu.org/licenses/>.
#

# Compiler
CC = g++

# Output from wx-config --libs
# Make sure the path to wxwidget\lib\gcc_dll or like has been added
#   to LIBRARY_PATH
WXCONFIGLIBS = -mthreads -lwxmsw29u -lwxtiff -lwxjpeg -lwxpng -lwxzlib -lwxregexu -lwxexpat -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwxregexu -lwinspool -lwinmm -lshell32 -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 -ladvapi32 -lwsock32


# Output from wx-config --cxxflags
# Make sure the path to wxwidget\lib\gcc_dll\mswu and wxwidget\include
#   or like has been added to CPATH
WXCONFIGFLAGS = -mthreads -DHAVE_W32API_H -D__WXMSW__ -D__WXDEBUG__ -D_UNICODE -DWXUSINGDLL -Wno-ctor-dtor-privacy -pipe -fmessage-length=0


# Compiler parameters
CFLAGS = -Os -Wl,-subsystem,windows -Wall
# Compiler parameters for DEBUG
DEBUG = -g

# Directory of source files
SRCDIR = src

# Directory of resource
ARTDIR = art

# Directory of library
LIBDIR = lib

# Directory of .h files
INCDIR = include

# Directory of object files
BLDDIR = build

# Compiler will search for library and included files in this setting
VPATH  = $(SRCDIR) : $(INCDIR)

# Name of output file (EXE)
DEST   = crystal-orientation-data-collection.exe


# List of all .cpp files (no extension required)
CPPS   = \
		main/dialog \
		main/app \
		main/taskbaricon \
		main/client_connection \
		main/client \
		main/server_connection \
		main/server \
		main/confirm_dialog \
		main/process_controller \


# Work out a list of all source files
SRCS   = $(addprefix $(SRCDIR)/,  $(addsuffix .cpp, $(CPPS)))

# Work out a list of all object files
OBJS   = $(addprefix $(BLDDIR)/,  $(addsuffix .o, $(CPPS)))

# .RC file to be linked
RCFILE = $(SRCDIR)/main/app.rc

# Object file of resource
RCOBJ  = $(BLDDIR)/main/app-res.o


# Default rule, build and run
default: compile run

$(SRCDIR)/main/dialog.cpp: $(ARTDIR)/icon.xpm

# Logic Rule: setup the dependence between object file and it's source file
$(OBJS): $(BLDDIR)/%.o : $(SRCDIR)/%.cpp : $(INCDIR)/%.h : $(INCDIR)/main/app.h
	@test -d $(subst /,\, $(dir $@)) || mkdir $(subst /,\, $(dir $@))
	$(CC) $(CFLAGS) -c -I$(SRCDIR) -I$(INCDIR) -I$(ARTDIR) $(WXCONFIGFLAGS) -o $@ $< -L$(LIBDIR) $(WXCONFIGLIBS)

# Logic Rule: setup the dependence between RES object file and it's artificals
$(RCOBJ): $(RCFILE)
	@echo #Rebuild Resources
	windres -O COFF -o $(RCOBJ) $(RCFILE) 

	
# Link and produce EXE file
compile : $(OBJS) $(RCOBJ)
	$(CC) $(CFLAGS) $(WXCONFIGFLAGS) -o $(BLDDIR)/$(DEST) $(OBJS) $(RCOBJ) -L$(LIBDIR) $(WXCONFIGLIBS)
	@echo # Compile done

# Remove all object files
clean-objs:
	@rm -f $(OBJS)

# Remove exe file
clean-dest:
	@rm -f $(BLDDIR)/$(DEST)

# Remove RC object file
clean-rc:
	@rm -f $(RCOBJ)

# Remove everything
clean: clean-objs clean-dest clean-rc
	@rm -f ./tags
	@echo #Cleaned

# build the ctags
ctags:
	@echo #Rebuild CTAGS
	ctags -f tags -R --exclude=*.svn* $(INCDIR) $(SRCDIR)

# Reset the config file and Execute
run:
	-mkdir $(BLDDIR)\config
	-copy /y config\sample.ini $(BLDDIR)\config\default.ini
	@$(BLDDIR)/$(DEST)
