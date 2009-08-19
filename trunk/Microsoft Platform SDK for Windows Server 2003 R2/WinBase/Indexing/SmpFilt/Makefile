# THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
# ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
# PARTICULAR PURPOSE.
#
# Copyright (c) Microsoft Corporation. All rights reserved.

!include <Win32.Mak>

all:    $(OUTDIR) $(OUTDIR)\SmpFilt.Dll

$(OUTDIR) :
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)

objs =  $(OUTDIR)\SmpFilt.Obj

.Cxx{$(OUTDIR)}.Obj:
    $(cc) $(cflags) $(cvarsdll) $(cdebug) -Dtry=try /EHsc /GR -DWINNT=1 -DUNICODE /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" /D_CRT_SECURE_NO_DEPRECATE $**

SmpFilt.Exp: $(objs) SmpFilt.Def
    $(implib) -machine:$(CPU) -def:SmpFilt.Def $(objs) -out:SmpFilt.Lib \
        OldNames.Lib  \
        Kernel32.Lib

$(OUTDIR)\SmpFilt.Dll: $(objs) 
    $(link) $(dlllflags) $(ldebug) -out:$(OUTDIR)\SmpFilt.Dll $(objs) /def:SmpFilt.def $(olelibs)

clean:
        $(CLEANUP)
