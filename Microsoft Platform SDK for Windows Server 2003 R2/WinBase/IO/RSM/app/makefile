#----- Include the PSDK's WIN32.MAK to pick up defines-------------------
!include <win32.mak>



all:	$(OUTDIR) $(OUTDIR)\app.exe 

$(OUTDIR)\app.exe:	 $(OUTDIR) $(OUTDIR)\App.obj $(OUTDIR)\App_Utls.obj $(OUTDIR)\STL_write.obj 
	$(link) $(ldebug) $(guilibs) /OUT:$(OUTDIR)\app.exe /NODEFAULTLIB:LIBC.LIB /PDB:$(OUTDIR)\app.pdb /MACHINE:$(CPU) $(OUTDIR)\App.obj $(OUTDIR)\App_Utls.obj $(OUTDIR)\STL_write.obj kernel32.lib user32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ntmsapi.lib 

$(OUTDIR)\App.obj:	App.cpp
	$(cc) $(cflags) $(cvarsdll) $(cdebug) /EHsc /Fp$(OUTDIR)\app.pch /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" /D"_CONSOLE" /D"_MBCS" App.cpp

$(OUTDIR)\App_Utls.obj:	App_Utls.cpp
	$(cc) $(cflags) $(cvarsdll) $(cdebug) /EHsc /Fp$(OUTDIR)\app.pch /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" /D"_CONSOLE" /D"_MBCS" App_Utls.cpp

$(OUTDIR)\STL_write.obj:	STL_write.cpp
	$(cc) $(cflags) $(cvarsdll) $(cdebug) /EHsc /Fp$(OUTDIR)\app.pch /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" /D"_CONSOLE" /D"_MBCS" STL_write.cpp


#----- If OUTDIR does not exist, then create directory
$(OUTDIR) :
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)

clean:
        $(CLEANUP)
