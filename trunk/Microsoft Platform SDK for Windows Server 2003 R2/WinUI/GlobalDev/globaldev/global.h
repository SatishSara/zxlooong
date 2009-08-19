///////////////////////////////////////////////////////////////////////////////////////////
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
//
// Copyright 2001 Microsoft Corporation.  All Rights Reserved.
//
// Purpose: 
//      This application demonstrates the different aspects of software globalization:
//
//      - A single executable with world-wide functionality (fully Unicode enabled
//      for Windows 2000 and Windows XP).
//
//      - Locale independency by using Windows NLS APIs to display data (dates,
//      numbers, time, etc.) within a user's desired format.
//
//      - Multilingual editing using standard edit controls.
//
//      - Multilingual input locales within edit control fields.
//
//      - Font selection and their affect on displayed data.
//
//      - Multilingual user interface and satellite DLLs
//
// Written by Houman Pournasseh
//
///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
//                                                                             
//   Module:        GLOBAL.H
//                                                                              
//   Description:    General include file used by all modules.
//                                                                              
///////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <commdlg.h>
#include <winuser.h>
#include <windowsx.h>
#include <dos.h>
#include <tchar.h>
#include <io.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cpl.h>
#include <time.h>
#include <commctrl.h>
#include <direct.h>
#include <objbase.h>
#include <shlobj.h>
#include <TCHAR.h>

#include "resource.h"
#include "res.h"

// global defines...
#define WS_EX_LAYOUTRTL                         0x00400000L // Right to left mirroring
#define WS_EX_NOINHERITLAYOUT                   0x00100000L // Disable inheritence of mirroring by children

#define STR_LEN     50
#define MAX_STR     256
#define STR_CPLEN   5
#define NBR_PS_PAGE 3

#define COLUMN_LANGID        0
#define COLUMN_LANGUAGE      1
#define COLUMN_NATIVELANG    2
#define COLUMN_NATIVECOUNTRYREGION 3
#define COLUMN_ACP           4
#define COLUMN_OEMCP         5

#define DEFAULT_FONT    TEXT("MS Shell Dlg")


// structure definitions...
typedef struct _LOCINFO                   // structure for locale related information displayed in our list view
{   
    TCHAR    tcLcid[STR_LEN];             // LCID
    TCHAR    tcLLang[STR_LEN];            // localized language name
    TCHAR    tcNLang[STR_LEN];            // native language name
    TCHAR    tcNCountryRegion[STR_LEN];   // native country/region name    
    TCHAR    tcAcp[STR_CPLEN];            // ANSI code page
    TCHAR    tcOem[STR_CPLEN];            // OEM code page
} LOCINFO,*LPLOCINFO; 


HINSTANCE    g_hInst;                // Handle to this instance
TCHAR        g_tcsTemp[MAX_STR];
HANDLE       g_hRes;                 // Handle to our resource dll
int          g_wCurLang;

// Function declarations...
INT_PTR CALLBACK FontDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK NLSDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK TextDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK USPDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK MirroringDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK GeneralDlgProc(HWND, UINT, WPARAM, LPARAM);

// Just a handy macro for deleting a font in a control when the
// dialog is about to shut down or change fonts.
#define DeleteFontObject(_hDlg, _hFont, _CntlID) \
do \
{ \
    _hFont = (HFONT) SendDlgItemMessage(_hDlg, _CntlID, WM_GETFONT, \
        (WPARAM) 0,  (LPARAM) 0) ; \
    if (_hFont) DeleteObject (_hFont) ; \
}while (0);
