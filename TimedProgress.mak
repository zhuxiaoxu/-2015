# Microsoft Developer Studio Generated NMAKE File, Based on TimedProgress.dsp
!IF "$(CFG)" == ""
CFG=TimedProgress - Win32 Debug
!MESSAGE No configuration specified. Defaulting to TimedProgress - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "TimedProgress - Win32 Release" && "$(CFG)" != "TimedProgress - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TimedProgress.mak" CFG="TimedProgress - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TimedProgress - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TimedProgress - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TimedProgress - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\TimedProgress.exe"


CLEAN :
	-@erase "$(INTDIR)\Ana_file.obj"
	-@erase "$(INTDIR)\BJDATA.obj"
	-@erase "$(INTDIR)\ProgressTimeToComplete.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\symnclass.obj"
	-@erase "$(INTDIR)\TimedProgress.obj"
	-@erase "$(INTDIR)\TimedProgress.pch"
	-@erase "$(INTDIR)\TimedProgress.res"
	-@erase "$(INTDIR)\TimedProgressDlg.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\TimedProgress.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\TimedProgress.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x809 /fo"$(INTDIR)\TimedProgress.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TimedProgress.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\TimedProgress.pdb" /machine:I386 /out:"$(OUTDIR)\TimedProgress.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ProgressTimeToComplete.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TimedProgress.obj" \
	"$(INTDIR)\TimedProgressDlg.obj" \
	"$(INTDIR)\TimedProgress.res" \
	"$(INTDIR)\symnclass.obj" \
	"$(INTDIR)\BJDATA.obj" \
	"$(INTDIR)\Ana_file.obj"

"$(OUTDIR)\TimedProgress.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TimedProgress - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\TimedProgress.exe"


CLEAN :
	-@erase "$(INTDIR)\Ana_file.obj"
	-@erase "$(INTDIR)\BJDATA.obj"
	-@erase "$(INTDIR)\ProgressTimeToComplete.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\symnclass.obj"
	-@erase "$(INTDIR)\TimedProgress.obj"
	-@erase "$(INTDIR)\TimedProgress.pch"
	-@erase "$(INTDIR)\TimedProgress.res"
	-@erase "$(INTDIR)\TimedProgressDlg.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\TimedProgress.exe"
	-@erase "$(OUTDIR)\TimedProgress.ilk"
	-@erase "$(OUTDIR)\TimedProgress.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\TimedProgress.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x809 /fo"$(INTDIR)\TimedProgress.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TimedProgress.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\TimedProgress.pdb" /debug /machine:I386 /out:"$(OUTDIR)\TimedProgress.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ProgressTimeToComplete.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TimedProgress.obj" \
	"$(INTDIR)\TimedProgressDlg.obj" \
	"$(INTDIR)\TimedProgress.res" \
	"$(INTDIR)\symnclass.obj" \
	"$(INTDIR)\BJDATA.obj" \
	"$(INTDIR)\Ana_file.obj"

"$(OUTDIR)\TimedProgress.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("TimedProgress.dep")
!INCLUDE "TimedProgress.dep"
!ELSE 
!MESSAGE Warning: cannot find "TimedProgress.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "TimedProgress - Win32 Release" || "$(CFG)" == "TimedProgress - Win32 Debug"
SOURCE=.\Ana_file.cpp

"$(INTDIR)\Ana_file.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TimedProgress.pch"


SOURCE=.\BJDATA.cpp

"$(INTDIR)\BJDATA.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TimedProgress.pch"


SOURCE=.\ProgressTimeToComplete.cpp

"$(INTDIR)\ProgressTimeToComplete.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TimedProgress.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "TimedProgress - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\TimedProgress.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\TimedProgress.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TimedProgress - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\TimedProgress.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\TimedProgress.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\symnclass.cpp

"$(INTDIR)\symnclass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TimedProgress.pch"


SOURCE=.\TimedProgress.cpp

"$(INTDIR)\TimedProgress.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TimedProgress.pch"


SOURCE=.\TimedProgress.rc

"$(INTDIR)\TimedProgress.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\TimedProgressDlg.cpp

"$(INTDIR)\TimedProgressDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TimedProgress.pch"



!ENDIF 

