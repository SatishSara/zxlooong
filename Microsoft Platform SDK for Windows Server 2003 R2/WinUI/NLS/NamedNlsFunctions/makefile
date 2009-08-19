# Copyright Microsoft Corporation.  All Rights Reserved.

#----- Include the PSDK's WIN32.MAK to pick up defines------------------------------------
!include <win32.mak>

#----- OUTDIR is defined in WIN32.MAK This is the name of the destination directory-------
all: $(OUTDIR)\NamedNlsFunctions.exe 

LINK32_OBJS= \
    $(OUTDIR)\NamedNlsFunctions.Obj  \
        
#----- If OUTDIR does not exist, then create directory
$(OUTDIR) :
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)

# Generic rule for building ALL CPP files and placing their OBJ's in the OUTDIR
.cpp{$(OUTDIR)}.obj:
    $(cc) $(cdebug) $(cflags) $(cvarsmt) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $**
    
# Build rule for exe
$(OUTDIR)\NamedNlsFunctions.exe: $(OUTDIR) $(LINK32_OBJS) 
    $(link) $(ldebug) $(LINK32_OBJS) \
    -out:$(OUTDIR)\NamedNlsFunctions.exe

# Rules for cleaning out those old files
clean:
        $(CLEANUP)