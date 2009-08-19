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
// This application demonstrates the different aspects of software globalization:
//
//      A single executable with world-wide functionality (fully Unicode enabled
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
//      - Multilingual user interface and satellite DLLs.
//
// Written by Houman Pournasseh
//
///////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////
//                                                                             
//   Module:    GLOBAL.C                                                        
//                                                                              
//   Description:   Main function and entry point of Globaldev application. 
//                                                                              
//   Functions:     
//     _tWinMain()              - Main function.
//     CreatePropSheet()        - Creates the property container and adds PS Pages.
//     LoadLanguageResource()   - Initialize and loads the appropriate language DLL.
//                                                                              
///////////////////////////////////////////////////////////////////////////////////////////

#include "global.h"


// Declaration of internal functions
BOOL    CreatePropSheet(HWND);
HANDLE  LoadLanguageResource(WORD);


///////////////////////////////////////////////////////////////////////////////////////////
//  Function:       _tWinMain
//
//  Description:    Program entry point - init application, invoke the creation of
//                  property sheet, and do cleanup on exit.
//
//  Comments:       Windows 2000 and Windows XP are the only platforms really designed for 
//                  multilingual computing and therefore this will only run on top of these
//                  platforms. However, the principe and coding can be used on other platforms.
//
///////////////////////////////////////////////////////////////////////////////////////////
int WINAPI _tWinMain(HINSTANCE  hInstance, HINSTANCE  hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow)
{
    INITCOMMONCONTROLSEX    InitCtrls = {0};
    OSVERSIONINFO           VersionInfo = {0};
    BOOL                    bSuccess = TRUE;

    // Keep a handle to our main instance. That will be handy.
    g_hInst = hInstance;
    
    // Register specific common controls classes.
    InitCtrls.dwSize = sizeof (INITCOMMONCONTROLSEX);
    InitCtrls.dwICC = ICC_BAR_CLASSES ;
    InitCommonControlsEx(&InitCtrls);

    // We only run on top of Win2000 and Windows XP.
    // These are the only two platfroms really designed for multilingual computing
    VersionInfo.dwOSVersionInfoSize = sizeof (VersionInfo);
    GetVersionEx(&VersionInfo);
    if (VersionInfo.dwMajorVersion < 5)
    {
        MessageBox(NULL,TEXT("This application is designed to run on Windows 2000 and beyond."), TEXT("Global - Error to initialize"), MB_OK|MB_ICONSTOP);
        bSuccess =  FALSE;
    }
    else
    {

        // Load the appropriate resource DLL.
        // If a language is specefied at the command line, then load that language, if not,
        // select the resource DLL that matches the system UI language.
        if (_tcslen(lpszCmdLine) != 0)
        {
            // we start the application in a given language...
            g_hRes = LoadLanguageResource((LANGID) _tcstoul(lpszCmdLine, NULL, 16));
        }
        else
        {
            g_hRes = LoadLanguageResource(0);
        }

        // We fail to load any language DLL, just let the user know and end program.
        if (!g_hRes)
        {
            TCHAR   tcsErr[MAX_STR];
            LoadString(g_hInst, STR_RESDLL_NOTFOUND, tcsErr, MAX_STR);
            LoadString(g_hInst, STR_APPTITLE, g_tcsTemp, MAX_STR);
            MessageBox(NULL, tcsErr, g_tcsTemp, MB_OK|MB_ICONERROR);
            bSuccess =  FALSE;
        }
        else
        {
            // Create our application's proprety sheet.
            CreatePropSheet(NULL);
            // Free resource dll before leaving...
            bSuccess = FreeLibrary(g_hRes);
        }
    }
    return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////////////////
//  Function        CreatePropSheet
//
//  Description:    Create the PS container and adds adds the pages defined in the 
//                  specified property sheet header structure. 
//
//  Comments:
//
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CreatePropSheet(HWND hDlg)
{
    PROPSHEETPAGE       psp[NBR_PS_PAGE];
    PROPSHEETHEADER     psh = {0};
    UINT                uiPage = 0;

    // Create our "NLS API" page.
    // the resource template is coming from the resource DLL loaded at run time.
    psp[uiPage].dwSize      = sizeof(PROPSHEETPAGE);
    psp[uiPage].dwFlags     = PSP_DEFAULT;
    psp[uiPage].hInstance   = g_hRes;
    psp[uiPage].pszTemplate = MAKEINTRESOURCE(DLG_NLS);
    psp[uiPage].pszIcon     = NULL;
    psp[uiPage].pfnDlgProc  = NLSDlgProc;
    psp[uiPage].pszTitle    = NULL;
    psp[uiPage].lParam      = 0L;
    psp[uiPage].pfnCallback = NULL;
    uiPage++;

    // Create our "TEXT API" page.
    // The resource template is coming from the resource DLL loaded at run time.
    psp[uiPage].dwSize      = sizeof(PROPSHEETPAGE);
    psp[uiPage].dwFlags     = PSP_DEFAULT;
    psp[uiPage].hInstance   = g_hRes;
    psp[uiPage].pszTemplate = MAKEINTRESOURCE(DLG_TEXT);
    psp[uiPage].pszIcon     = NULL;
    psp[uiPage].pfnDlgProc  = TextDlgProc;
    psp[uiPage].pszTitle    = NULL;
    psp[uiPage].lParam      = 0L;
    psp[uiPage].pfnCallback = NULL;
    uiPage++;
    
    // Create our "General" page.
    // The resource template is coming from the resource DLL loaded at run time.
    psp[uiPage].dwSize      = sizeof(PROPSHEETPAGE);
    psp[uiPage].dwFlags     = PSP_DEFAULT;
    psp[uiPage].hInstance   = g_hRes;
    psp[uiPage].pszTemplate = MAKEINTRESOURCE(DLG_GENERAL);
    psp[uiPage].pszIcon     = NULL;
    psp[uiPage].pfnDlgProc  = GeneralDlgProc;
    psp[uiPage].pszTitle    = NULL;
    psp[uiPage].lParam      = 0L;
    psp[uiPage].pfnCallback = NULL;
    uiPage++;

    psh.dwSize      = sizeof(PROPSHEETHEADER);
    psh.dwFlags     = PSH_PROPSHEETPAGE | PSH_USEPAGELANG;
    psh.hwndParent  = hDlg;
    psh.hInstance   = g_hInst;
    psh.hIcon       = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_MAINICON));

    LoadString(g_hInst, STR_TITLE, g_tcsTemp, MAX_STR);
    psh.pszCaption  = g_tcsTemp;
    psh.nPages      = NBR_PS_PAGE;
    psh.nStartPage  = 0;
    psh.ppsp        = (LPCPROPSHEETPAGE)&psp;
    psh.pfnCallback = NULL;

    return (PropertySheet(&psh) != -1);
}

///////////////////////////////////////////////////////////////////////////////////////////
//  Function:       LoadLanguageResource
//
//  Description:    Decide which language resource DLL should be loaded.
//
//  Comments:       The best practice in naming resource DLLs is to go with the LangID
//                  since Win32 APIs would return a langID for the currently selected UI
//                  langauge.
//                  To learn more about how to retrieve the UI language on other platforms 
//                 (other than Win2000 and WinXP, check out: 
//                  http://www.microsoft.com/globaldev/articles/muiapp.asp
//
///////////////////////////////////////////////////////////////////////////////////////////
HANDLE  LoadLanguageResource(WORD wLangId)
{
    HANDLE  hRes = NULL;

    // if wLangId = NULL then load user's language of the UI
    // GetUserDefaltUILanguage only works on WinME, Win2000, and WinXP.
    // To retrieve the UI language on any other OS, refer to the article at:
    // http://www.microsoft.com/globaldev/articles/muiapp.asp
    if (wLangId == 0)
    {
        wLangId = GetUserDefaultUILanguage();
    }

    // Our naming convention for satellite DLLs is: gres[langID].dll
    // Create file name and load the appropriate resource DLL.
    _sntprintf(g_tcsTemp,MAX_STR,TEXT("gres%x.dll"), wLangId);
    g_wCurLang = wLangId;

    if(NULL == (hRes = LoadLibrary(g_tcsTemp)))
    {
        // We didn't find the desired language satellite DLL, lets go with English
        hRes = LoadLibrary(TEXT("gres409.dll"));

        // Keep the a copy of our UI language ID (will be handy).
        g_wCurLang = 409;
    }
    
     return hRes;
}
