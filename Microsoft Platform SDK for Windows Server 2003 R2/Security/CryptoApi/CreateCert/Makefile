#======================================================================
# THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
# KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
# PURPOSE.
#
# Copyright 1998-1999. Microsoft Corporation. All Right Reserved.
#
# File: MAKEFILE
#
# NMAKE description file for the CreateCert sample
#
#======================================================================
!IF "$(TARGETOS)" == "WINNT"

!include <win32.mak>

PROJ = CreateCert

all: $(OUTDIR) $(OUTDIR)\$(PROJ).exe

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"


#======================================================================
# Define project specific macros
#======================================================================
PROJ_OBJS  = $(OUTDIR)\$(PROJ).obj
EXTRA_LIBS = crypt32.lib rpcrt4.lib 


#======================================================================
# Define UNICODE if you want this.  UNICODE does not work on Win9x
#======================================================================
# EXTRA_DEFS = -DUNICODE -D_UNICODE

# DEF_FILE = -def:$(PROJ).def


#======================================================================
# Inference rule for updating the object files
#======================================================================
$(OUTDIR)\$(PROJ).obj : $(PROJ).c
   $(cc) $(cdebug) $(cflags) $(cvarsdll) $(EXTRA_DEFS) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $(PROJ).c


#======================================================================
# Build Rules for EXE
#======================================================================
$(OUTDIR)\$(PROJ).EXE : $(BASE_OBJS) $(PROJ_OBJS)
   $(link) $(ldebug) $(conlflags) $(PROJ_OBJS)\
   $(conlibs) $(EXTRA_LIBS) -out:$(OUTDIR)\$(PROJ).exe /PDB:$(OUTDIR)\$(PROJ).PDB

clean:
	$(CLEANUP)
!ELSE
!MESSAGE Sorry this sample only builds for the Windows NT Platform
!ENDIF
