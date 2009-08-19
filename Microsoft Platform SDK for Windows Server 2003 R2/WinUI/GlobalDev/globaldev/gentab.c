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
//  This application demonstrates the different aspects of software globalization:
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
//      - Multilingual user interface and satellite DLLs.
//
// Written by Houman Pournasseh
//
///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
//                                                                             
//   Module:        GENTAB.C                                                        
//                                                                              
//   Description:   Handles the window proc for the "General" tab of the property sheet.
//                                                                              
//   Functions:
//      GeneralDlgProc()        - Window procedure message handler
//      InitLangList()          - lists languages for which we have a resource DLL available
//                                                                              
///////////////////////////////////////////////////////////////////////////////////////////

#include "global.h"


// Internal function definition
void InitLangList(HWND);

///////////////////////////////////////////////////////////////////////////////////////////
//  Function:       GeneralDlgProc
//
//  Description:    Message-processing function for general tab
//
//  Comments:
//
///////////////////////////////////////////////////////////////////////////////////////////
INT_PTR CALLBACK GeneralDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int      nIndex;
    static   LCID  wNewLang;
    BOOL     bSuccess = TRUE;
   
    switch(uMsg)
    {
            case WM_INITDIALOG:
             {
                   HANDLE hFile = NULL;
                   LPTSTR lpszText = 0;
                   DWORD  dwReadByte = 0;
                   DWORD  dwFileSize = 0;

                    // We are displaying the content of global.txt in an edit control on this page.
                    // Load the file and set the text.
                    if((hFile = CreateFile(TEXT("global.txt"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
                    {
                        dwFileSize = GetFileSize(hFile, &dwReadByte);
                        if(NULL==(lpszText = (LPTSTR)VirtualAlloc(NULL, dwFileSize, MEM_COMMIT, PAGE_READWRITE)))
                        {
                            bSuccess = FALSE;
                            break;
                        }

                        ReadFile(hFile, lpszText, dwFileSize, &dwReadByte, NULL); 
                        if(FALSE == CloseHandle(hFile))
                        {
                            bSuccess = FALSE;
                            VirtualFree(lpszText, dwFileSize, MEM_RELEASE);
                            break;
                        }

                        SetWindowText(GetDlgItem(hDlg, IDC_PURPOSE), lpszText);
                        VirtualFree(lpszText, dwFileSize, MEM_RELEASE);
                    }
                    else
                    {
                        bSuccess = FALSE;
                        break;
                    }

                    // Create the list of the available languages to be displayed in our
                    // drop-down menu for UI language selection
                    InitLangList(hDlg);
            }
            break;

            case WM_COMMAND:
                switch(LOWORD(wParam))
                {
                    case IDCANCEL:  /* fall-through*/
                    case IDOK:
                        break;
                }

                switch (HIWORD(wParam)) 
                {
                    case CBN_SELCHANGE:  /* fall-through*/
                    case CBN_DBLCLK:

                    // track the user changes into the language selection drop-down menu.
                    nIndex = (int)SendMessage((HWND) lParam, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);

                    if(CB_ERR == nIndex)
                    {
                        bSuccess = FALSE;
                        break;
                    }

                    wNewLang = (LCID) SendMessage((HWND) lParam, CB_GETITEMDATA, (WPARAM) nIndex, (LPARAM) 0 );
                    if (wNewLang != (LCID)g_wCurLang)
                    {
                        // enable the apply buton...
                        SendMessage(GetParent(hDlg), PSM_CHANGED, (WPARAM)hDlg, 0L);   // enable Apply
                    }
                    else
                    {
                        SendMessage(GetParent(hDlg), PSM_UNCHANGED, (WPARAM)hDlg, 0L);   // enable Apply
                    }
                    break;
                }
                break;

            case WM_NOTIFY:
                switch (((NMHDR *)lParam)->code)
                {
                    case PSN_APPLY:
                    {
                        PSHNOTIFY *lppsn = NULL;

                        lppsn = (LPPSHNOTIFY) lParam; 
                        if (lppsn->lParam == FALSE)
                        {
                            // we have an apply, we need to re-load new langauge resources...
                        
                            STARTUPINFO         sui = {0};
                            PROCESS_INFORMATION pi1 = {0};
                            TCHAR               szProcess[MAX_STR];

                            // Create a new instance on ourselve and pass the new language
                            // dll as a parameter.
                            sui.cb = sizeof (STARTUPINFO);
                            _sntprintf(szProcess,MAX_STR,TEXT("global %x"), wNewLang);
							bSuccess = CreateProcess(NULL, szProcess, 0, 0, 0, 0, 0, 0, &sui, &pi1);

                            // close the previous version...
                            PostMessage(GetParent(hDlg), WM_CLOSE, 0, 0);
                        }
                    }
                    break;
                }
                break;

        default:
         bSuccess = FALSE;
   }
    
    return bSuccess;
}


///////////////////////////////////////////////////////////////////////////////////////////
//  Function:       InitLangList
//
//  Description:    Makes a list of available language DLLs and fills out the language
//                  selection drop-down menu.
//  Comments:
//
///////////////////////////////////////////////////////////////////////////////////////////
void InitLangList(HWND hDlg)
{
    int   nIndex = 0;
    WIN32_FIND_DATA wfd = {0};
    HANDLE hFindFile = NULL;

    // Our naming convention for resource DLLs is as follow: gres[langID].dll
    // Find all available resource dlls in the current directory but enumerating gres*.* files
    hFindFile = FindFirstFile(TEXT("gres*.*"), &wfd);

    if(NULL != hFindFile)
    {
        do
        {
            LANGID wFileLang = 0;
            TCHAR szLangName[32];

            // Skip first four letters ("GRES") of filename, convert the rest to a langID.
            wFileLang = (LANGID) _tcstoul(wfd.cFileName+4, NULL, 16);

            // We only offer UI languages that are in fact supported...
            // This is done to prevent users from selecting a language for which the support (font, USP...)
            // are not installed.
            if (IsValidLocale(wFileLang, LCID_INSTALLED))
            {
                // Get the native name of the country associated with that language ID.
                GetLocaleInfo(MAKELCID(wFileLang, SORT_DEFAULT) , LOCALE_SNATIVELANGNAME, szLangName, 32);

                // Add the new language to our list of UI languages.
                SendDlgItemMessage(hDlg, IDC_LANGUAGES, CB_INSERTSTRING, nIndex, (LPARAM) szLangName);

                // Store the langID of the current resource DLL in the combo-box data area
                // for later use.
                SendDlgItemMessage(hDlg, IDC_LANGUAGES, CB_SETITEMDATA, nIndex, (LPARAM) wFileLang);

                // Select the current language in the combo box edit control.
                if(wFileLang == g_wCurLang)
                {
                    SendDlgItemMessage(hDlg, IDC_LANGUAGES, CB_SETCURSEL, nIndex, 0L );
                }

                nIndex++;
            }
        }
        // Look for the followig resouce DLL 
        while (FindNextFile(hFindFile, &wfd) );  // Test for do ... while loop
    }
}
