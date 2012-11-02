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

# Environment 
OS      := $(shell uname -s)

# Commands
MKDIR := $(shell which mkdir) -p
RM    := $(shell which rm) -rf
CP    := $(shell which cp)

# environment variable WXWIN for wx-config
WXWIN   ?= .

# Output from wx-config --libs
# Make sure the path to wxwidget\lib\gcc_dll or like has been added
#   to LIBRARY_PATH
WXCONFIGLIBS = $(shell wx-config --libs)


# Output from wx-config --cxxflags
# Make sure the path to wxwidget\lib\gcc_dll\mswu and wxwidget\include
#   or like has been added to CPATH
WXCONFIGFLAGS = $(shell wx-config --cxxflags)


# Compiler parameters
ifeq ($(OS),Linux)
CFLAGS := -Wall
else ifeq ($(OS),windows32)
CFLAGS := -Wl,-subsystem,windows -Wall
endif

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
BLDDIR = build-$(OS)

# Compiler will search for library and included files in this setting
VPATH  = $(SRCDIR) : $(INCDIR)

# Directory of distrib
DISTRIBDIR = distrib

# Name of output file (EXE)
DEST   = CRYSTOsoftware.exe


# List of all .cpp files (no extension required)
CPPS   = \
		main/app \
		main/async_process \
		main/client \
		main/client_connection \
		main/confirm_dialog \
		main/convertdata_process \
		main/datafile_monitor \
		main/datafile_storage \
		main/dialog \
		main/export_process \
		main/launchequipment_process \
		main/macro_scheduler \
		main/notify_dialog \
		main/process_controller \
		main/project_updater \
		main/server \
		main/server_connection \
		main/taskbaricon \
		main/transfer_process \
		main/update_process \
		main/import_project_dialog \



# Work out a list of all source files
SRCS   = $(addprefix $(SRCDIR)/,  $(addsuffix .cpp, $(CPPS)))

# Work out a list of all object files
OBJS   = $(addprefix $(BLDDIR)/,  $(addsuffix .o, $(CPPS)))

# .RC file to be linked
RCFILE = $(SRCDIR)/main/app.rc

# Object file of resource
RCOBJ  = $(BLDDIR)/main/app-res.o


# Default rule, build and run
default: compile 

$(SRCDIR)/main/dialog.cpp: $(ARTDIR)/icon.xpm


# Logic Rule: setup the dependence between object file and it's source file
$(SRCS) : $(SRCDIR)/%.cpp : $(INCDIR)/%.h $(INCDIR)/main/app.h $(INCDIR)/main/define.h

$(OBJS) : $(BLDDIR)/%.o : $(SRCDIR)/%.cpp
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c -I$(SRCDIR) -I$(INCDIR) -I$(ARTDIR) $(WXCONFIGFLAGS) -o $@ $< -L$(LIBDIR) $(WXCONFIGLIBS)

# Logic Rule: setup the dependence between RES object file and it's artificals
$(RCOBJ): $(RCFILE)
	@echo #Rebuild Resources
	windres -O COFF -I$(WXWIN)/include -o $(RCOBJ) $(RCFILE) 

	
# Link and produce EXE file
compile : $(OBJS) $(RCOBJ)
	$(CC) $(CFLAGS) $(WXCONFIGFLAGS) -o $(BLDDIR)/$(DEST) $(OBJS) $(RCOBJ) -L$(LIBDIR) $(WXCONFIGLIBS)
	@echo # Compile done

# Remove everything
clean:
	@$(RM) $(BLDDIR)
	@echo #Cleaned

# build the ctags
ctags:
	@echo #Rebuild CTAGS
	ctags -f tags -R --exclude=*.svn* $(INCDIR) $(SRCDIR)

clearcache:
	@echo Removing Project Cache
	@$(RM) $(BLDDIR)/projects

# Reset the config file and Execute
run:
	@echo Updating utils
	@$(CP) -r utils $(DISTRIBDIR)

	@echo Updating config files
	@$(CP) -r config $(DISTRIBDIR)

	@echo Copying execution
	@$(CP) $(BLDDIR)/$(DEST) $(DISTRIBDIR)/$(DEST)

	@echo Starting $(DISTRIBDIR)/$(DEST) ...
	@$(DISTRIBDIR)\$(DEST) "-c" "config/sample.ini"
