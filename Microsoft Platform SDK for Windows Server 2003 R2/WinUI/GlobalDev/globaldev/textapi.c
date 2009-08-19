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
//    This application demonstrates the different aspects of software globalization:
//
//        - A single executable with world-wide functionality (fully Unicode enabled
//        for Windows 2000 and Windows XP).
//
//        - Locale independency by using Windows NLS APIs to display data (dates,
//        numbers, time, etc.) within a user's desired format.
//
//        - Multilingual editing using standard edit controls.
//
//        - Multilingual input locales within edit control fields.
//
//        - Font selection and their affect on displayed data.
//
//        - Multilingual user interface and satellite DLLs.
//
// Written by Houman Pournasseh
//
///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
//                                                                             
//   Module:        TEXTAPI.C                                                        
//                                                                              
//   Description:    Handles the window proc for the "Text API" tab of the property sheet.
//                                                                              
//   Functions:        
//        TextDlgProc()     - Window procedure message handler.
//        InitializeFont()  - Fills in font structures with initial values. 
//                                                                              
///////////////////////////////////////////////////////////////////////////////////////////


#include "global.h"

// internal function declaration
void InitializeFont(HWND, LPTSTR, LONG, LPCHOOSEFONT, LPLOGFONT);

///////////////////////////////////////////////////////////////////////////////////////////
//    Function:        TextDlgProc
//
//    Description:    Message-processing function for text tab
//
//    Comments:        
//
///////////////////////////////////////////////////////////////////////////////////////////
INT_PTR CALLBACK TextDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG        lAlign = 0;
    HFONT       hEditFont = NULL;
    HWND        hWndEdit = NULL;
    static      CHOOSEFONT cf; 
    static      LOGFONT lf;
    INT_PTR     iProcessedMsg   = 1;


    switch(uMsg)
    {
        case WM_INITDIALOG:
            {
                // Fill out a font structure that will be used in a call for CreateFont.
                InitializeFont(hDlg, DEFAULT_FONT, 24, &cf, &lf);

                hEditFont = CreateFontIndirect(&lf) ;
                if(NULL == hEditFont)
                {
                    iProcessedMsg = 0;
                    break;
                }
                // Set font of edit control
                SendDlgItemMessage(hDlg, IDC_EDITWIN, WM_SETFONT, (WPARAM) hEditFont,  MAKELPARAM(TRUE, 0)) ;
            }
            break;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                // Erase our text control by setting the content to NULL.
                case IDB_CLEAR:
                    {
                        hWndEdit = GetDlgItem (hDlg, IDC_EDITWIN);
                        if(NULL == hWndEdit)
                        {
                            iProcessedMsg = 0;
                            break;
                        }
                        SetWindowText(hWndEdit, TEXT(""));
                    }
                    break;

                // Provide a font selection mechanism through the system's font selection
                // common dialog.
                case IDB_FONT:
                    {
                        DeleteFontObject(hDlg, hEditFont, IDC_EDITWIN);
                        
                        if(FALSE == ChooseFont(&cf))
                        {
                            iProcessedMsg = 0;
                            break;
                        }
                        
                        hEditFont = CreateFontIndirect(&lf);   
                        if(NULL == hEditFont)
                        {
                            iProcessedMsg = 0;
                            break;
                        }
                        SendDlgItemMessage (hDlg, IDC_EDITWIN, WM_SETFONT, (WPARAM) hEditFont,  MAKELPARAM(TRUE, 0));
                    }
                    break;

                // Toggles the reading order of the text.
                case IDB_READING:
                    {
                        hWndEdit = GetDlgItem(hDlg, IDC_EDITWIN);
                        if(NULL == hWndEdit)
                        {
                            iProcessedMsg = 0;
                            break;
                        }
                        lAlign   = GetWindowLong(hWndEdit, GWL_EXSTYLE) ^ WS_EX_RTLREADING;            
                        SetWindowLong(hWndEdit, GWL_EXSTYLE, lAlign); 
                        InvalidateRect(hWndEdit ,NULL, TRUE);
                    }
                    break;

                // Toggles the text alignment.
                case IDB_ALIGN:
                    {
                        hWndEdit = GetDlgItem(hDlg, IDC_EDITWIN);
                        if(NULL == hWndEdit)
                        {
                            iProcessedMsg = 0;
                            break;
                        }
                        lAlign   = GetWindowLong(hWndEdit, GWL_EXSTYLE) ^ WS_EX_RIGHT;            
                        SetWindowLong(hWndEdit, GWL_EXSTYLE, lAlign); 
                        InvalidateRect(hWndEdit ,NULL, TRUE);
                    }
                    break;

                case IDCANCEL: /* fall-through*/
                case IDOK:
                    {
                        DeleteFontObject(hDlg, hEditFont, IDC_EDITWIN);
                    }
                    break;
            }
         break;

        default:
            iProcessedMsg = 0;
            break;
    }
    return iProcessedMsg;
}

///////////////////////////////////////////////////////////////////////////////////////////
//    Function:        InitializeFont
//
//    Description:    Fills in font structures (CHOOSEFONT and a LOGFONT) with initial values. 
//
//    Comments:        
//
///////////////////////////////////////////////////////////////////////////////////////////
void InitializeFont(HWND hWnd, LPTSTR szFaceName, LONG lHeight, LPCHOOSEFONT lpCf, LPLOGFONT lpLf)
{
    lpCf->lStructSize   = sizeof(CHOOSEFONT) ;
    lpCf->hwndOwner     = hWnd ;
    lpCf->hDC           = NULL ;
    lpCf->lpLogFont     = lpLf ;
    lpCf->iPointSize    = 10;
    lpCf->Flags         = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_NOSIZESEL ;
    lpCf->rgbColors     = RGB(0,0,0);
    lpCf->lCustData     = 0;
    lpCf->lpfnHook      = NULL;
    lpCf->lpTemplateName= NULL;
    lpCf->hInstance     = g_hInst;
    lpCf->lpszStyle     = NULL;
    lpCf->nFontType     = SIMULATED_FONTTYPE;
    lpCf->nSizeMin      = 0;
    lpCf->nSizeMax      = 0;
    
    lpLf->lfHeight      = lHeight ; 
    lpLf->lfWidth       = 0 ; 
    lpLf->lfEscapement  = 0 ; 
    lpLf->lfOrientation = 0 ; 
    lpLf->lfWeight      = FW_DONTCARE ; 
    lpLf->lfItalic      = FALSE ; 
    lpLf->lfUnderline   = FALSE ; 
    lpLf->lfStrikeOut   = FALSE ; 
    lpLf->lfCharSet     = DEFAULT_CHARSET ; 
    lpLf->lfOutPrecision= OUT_DEFAULT_PRECIS ; 
    lpLf->lfClipPrecision = CLIP_DEFAULT_PRECIS ; 
    lpLf->lfQuality     = DEFAULT_QUALITY ; 
    lpLf->lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE ; 
    _tcsncpy(lpLf->lfFaceName, szFaceName,min(_tcslen(lpLf->lfFaceName),_tcslen(szFaceName)));
}
