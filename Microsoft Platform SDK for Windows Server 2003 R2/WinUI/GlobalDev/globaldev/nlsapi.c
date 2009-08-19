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
//   Module:        NLSAPI.C                                                        
//                                                                              
//   Description:    Handles the window proc for the "NLS API" tab of the property sheet.
//                                                                              
//   Functions:        
//        NLSDlgProc()          - Window procedure message handler.
//        LcidDlgProc()         - Window procedure message handler.
//        iConvertStrToInt()    - Converts a hex value saved as string to int.
//        InitNLSFields()       - Initialize NLS formatting fields for a given LCID.
//        EnumLocalesProc()     - Callback function for locale enumeration.
//        EnumTimeFormatsProc() - Callback function for time format enumeration.
//        EnumSDateFormatsProc()- Callback function for short date format enumeration.
//        EnumLDateFormatsProc()- Callback function for long date format enumeration.
//        EnumCalendarInfoProc()- Callback function for calendar information enumeration.
//        CompareFunc()         - A private compare function for list-view sorting.
//                                                                              
///////////////////////////////////////////////////////////////////////////////////////////
#include "global.h"

// internal function declaration
int     iConvertStrToInt(LPTSTR);
void    InitNLSFields(HWND, LCID);
BOOL    CALLBACK EnumLocalesProc(LPTSTR);
BOOL    CALLBACK EnumTimeFormatsProc(LPTSTR);
BOOL    CALLBACK EnumSDateFormatsProc(LPTSTR);
BOOL    CALLBACK EnumCalendarInfoProc(LPTSTR);
BOOL    CALLBACK EnumLDateFormatsProc(LPTSTR);
INT_PTR CALLBACK LcidDlgProc(HWND, UINT, WPARAM, LPARAM);
int     CALLBACK CompareFunc(LPARAM, LPARAM, LPARAM);

// general variable declaration
HWND    g_hwndList;
HWND    g_hDlg;
int     g_iCurLocale;
int     g_iCounter = 0;


///////////////////////////////////////////////////////////////////////////////////////////
//    Function:        NLSDlgProc
//
//    Description:    Message-processing function for text tab
//
//    Comments:        
//
///////////////////////////////////////////////////////////////////////////////////////////
INT_PTR CALLBACK NLSDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL bProcessedMsg = TRUE;
    
    switch(uMsg)
    {
        case WM_INITDIALOG:
        {
            LVCOLUMN    lvcCol = {0};
            UINT        lcid = 0;
            LVFINDINFO  lvInfo = {0};
            int         iItem = 0;

            // load our own icon
            SendMessage(GetParent(hDlg), WM_SETICON, ICON_BIG, (LPARAM) (LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_MAINICON))));

            // Create our list column  for the listview control
            g_hwndList = GetDlgItem(hDlg, IDC_LV_LCID);
            g_hDlg = hDlg;

            lvcCol.mask = LVCF_TEXT;
            LoadString(g_hRes, STR_LANGID, g_tcsTemp, MAX_STR);
            lvcCol.pszText = g_tcsTemp;
            ListView_InsertColumn(g_hwndList, COLUMN_LANGID, (LV_COLUMN *)&lvcCol);

            LoadString(g_hRes, STR_LANG, g_tcsTemp, MAX_STR);
            lvcCol.pszText = g_tcsTemp;
            ListView_InsertColumn(g_hwndList, COLUMN_LANGUAGE, (LV_COLUMN *)&lvcCol);

            LoadString(g_hRes, STR_NLANG, g_tcsTemp, MAX_STR);
            lvcCol.pszText = g_tcsTemp;
            ListView_InsertColumn(g_hwndList, COLUMN_NATIVELANG, (LV_COLUMN *)&lvcCol);

            LoadString(g_hRes, STR_NCOUNTRY, g_tcsTemp, MAX_STR);
            lvcCol.pszText = g_tcsTemp;
            ListView_InsertColumn(g_hwndList, COLUMN_NATIVECOUNTRYREGION, (LV_COLUMN *)&lvcCol);

            LoadString(g_hRes, STR_ACP, g_tcsTemp, MAX_STR);
            lvcCol.pszText = g_tcsTemp;
            ListView_InsertColumn(g_hwndList, COLUMN_ACP, (LV_COLUMN *)&lvcCol);

            LoadString(g_hRes, STR_OEMCP, g_tcsTemp, MAX_STR);
            lvcCol.pszText = g_tcsTemp;
            ListView_InsertColumn(g_hwndList, COLUMN_OEMCP, (LV_COLUMN *)&lvcCol);

            // fill out our listview control with available locales (supported ones).
            EnumSystemLocales(EnumLocalesProc, LCID_SUPPORTED);

            // Initialize formatting fields related to the selected locale in the listview
            InitNLSFields(hDlg, LOCALE_USER_DEFAULT);

            // Set the column width of the listview control.
            ListView_SetColumnWidth(g_hwndList, COLUMN_LANGID, LVSCW_AUTOSIZE_USEHEADER);
            ListView_SetColumnWidth(g_hwndList, COLUMN_LANGUAGE, LVSCW_AUTOSIZE_USEHEADER);
            ListView_SetColumnWidth(g_hwndList, COLUMN_NATIVELANG, LVSCW_AUTOSIZE_USEHEADER);
            ListView_SetColumnWidth(g_hwndList, COLUMN_NATIVECOUNTRYREGION, LVSCW_AUTOSIZE_USEHEADER);
            ListView_SetColumnWidth(g_hwndList, COLUMN_ACP, LVSCW_AUTOSIZE_USEHEADER);
            ListView_SetColumnWidth(g_hwndList, COLUMN_OEMCP, LVSCW_AUTOSIZE_USEHEADER);

            // Find user's current locale and select that item in the listview control.
            lcid = GetUserDefaultLCID();
            
            // Our list has all the LCIDs in 4 digits (0409) whereas GetUserDefaultLCID returs (409).
            if (lcid < 0x1000)
            {
                _sntprintf(g_tcsTemp,MAX_STR,TEXT("0%x"), lcid);
            }
            else
            {
                _sntprintf(g_tcsTemp,MAX_STR,TEXT("%x"), lcid);
            }

            lvInfo.flags = LVFI_STRING;
            lvInfo.psz = g_tcsTemp;

            iItem = ListView_FindItem(g_hwndList, 0, &lvInfo);
            ListView_SetItemState (g_hwndList, iItem, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
            ListView_EnsureVisible(g_hwndList, iItem, FALSE);

            // Set some of the extended styles of our list view (cool reports ;-)
            ListView_SetExtendedListViewStyle(g_hwndList, LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_GRIDLINES | LVS_EX_TWOCLICKACTIVATE);
        }
        break;

        case WM_NOTIFY:
            switch (((NMHDR *)lParam)->code)
            {
                case LVN_COLUMNCLICK:
                    {
                        NM_LISTVIEW nmv;
                        static int iLastSort = COLUMN_LANGID;
                        LPARAM lSortParam = -1;
                        LVFINDINFO lvInfo;
                        int         iItem;

                        // When a column is clicked we need to redo our sorting of preview table.

                        nmv = *(const NM_LISTVIEW *) lParam;

                        // We don't sort for native language and native country name (avoid confusion).
                        if ((nmv.iSubItem == COLUMN_NATIVELANG) || (nmv.iSubItem == COLUMN_NATIVECOUNTRYREGION))
                        {
                            break;
                        }

                        // If our last sorting was on a different column, we need to do more
                        // than simply reverting our sorting.
                        // Because of listview sorting limitations, we need to redo our list...
                        if (iLastSort != nmv.iSubItem)
                        {
                            // The list view sorting is based on a q-sort, we need to redo our list!
                            ListView_DeleteAllItems(g_hwndList);
                            g_iCounter  = 0;
                            EnumSystemLocales(EnumLocalesProc, LCID_SUPPORTED);

                            // Make sure the last selection remains active after sorting...
                            if (g_iCurLocale < 0x1000)
                            {
                                _sntprintf(g_tcsTemp,MAX_STR,TEXT("0%x"), g_iCurLocale);
                            }
                            else
                            {
                                _sntprintf(g_tcsTemp,MAX_STR,TEXT("%x"), g_iCurLocale);
                            }

                            lvInfo.flags = LVFI_STRING;
                            lvInfo.psz = g_tcsTemp;
                            iItem = ListView_FindItem(g_hwndList, 0, &lvInfo);
                            ListView_SetItemState (g_hwndList, iItem, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);

                            // Update our flag for the last column sorted and set the sorting flag
                            iLastSort = nmv.iSubItem;
                            lSortParam = iLastSort;
                        }

                        // Do the actual sorting...
                        ListView_SortItems(g_hwndList, (PFNLVCOMPARE)CompareFunc, lSortParam);

                        // Make sure our selection is visible after the sorting!
                        ListView_EnsureVisible(g_hwndList, ListView_GetSelectionMark(g_hwndList), FALSE);
                    }
                    break;

                case LVN_ITEMCHANGED:   /* fall-through */
                case LVN_ITEMACTIVATE:
                    {
                        LPNM_LISTVIEW lpnmlv = (LPNM_LISTVIEW)lParam;
                        static    int iItem;
                        LVITEM    lvItem = {0};

                        // Item selection has been changed.
                        // Find the new langId and update our formatting fields...
                        if (iItem != lpnmlv->iItem)
                        {
                            iItem = lpnmlv->iItem;

                            lvItem.mask = LVIF_TEXT;
                            lvItem.iItem = iItem;
                            lvItem.pszText = g_tcsTemp;
                            lvItem.cchTextMax  = STR_LEN;
                            ListView_GetItem(g_hwndList, &lvItem);                                                    
                            g_iCurLocale = iConvertStrToInt(lvItem.pszText);
                            InitNLSFields(hDlg, g_iCurLocale);
                        }

                        // Show advanced data on the selected locale...
                        if (((NMHDR *)lParam)->code == LVN_ITEMACTIVATE)
                        {
                           bProcessedMsg = (DialogBox(g_hRes, MAKEINTRESOURCE(DLG_LCID), hDlg, LcidDlgProc) != -1);
                        }
                    }
                    break;
                }
                break;

                case WM_COMMAND:
                    switch(LOWORD(wParam))
                    {
                        case IDCANCEL: /* fall-through*/
                        case IDOK:
                            break;

                        case IDC_TIMEFORMAT:
                            {
                                if(HIWORD(wParam) == CBN_SELCHANGE)
                                {
                                    // Time formating selection has been changed.
                                    // Re-edit time
                                    HWND    hList = NULL;
                                    LRESULT index = 0;
                                    TCHAR    tcTemp[MAX_STR];

                                    hList = GetDlgItem(hDlg, IDC_TIMEFORMAT);

                                    if(NULL == hList)
                                    {
                                        bProcessedMsg = FALSE;
                                        break;
                                    }

                                    index = SendMessage(hList, CB_GETCURSEL, 0, 0);
                                    SendMessage(hList, CB_GETLBTEXT, index, (LPARAM) g_tcsTemp);
                                    GetTimeFormat(g_iCurLocale, 0, NULL, g_tcsTemp, tcTemp, MAX_STR);
                                    SetWindowText(GetDlgItem(hDlg, IDC_TIMESAMPLE), tcTemp);
                                }
                            }
                            break;

                        case IDC_SDATEFORMAT:
                            {
                                if(HIWORD(wParam) == CBN_SELCHANGE)
                                {
                                    // short date formating selection has been changed.
                                    // Re-edit short date
                                    HWND    hList = NULL;
                                    LRESULT index = 0;
                                    TCHAR    tcTemp[MAX_STR];

                                    hList = GetDlgItem(hDlg, IDC_SDATEFORMAT);
                                    
                                    if(NULL == hList)
                                    {
                                        bProcessedMsg = FALSE;
                                        break;
                                    }
                                    index = SendMessage(hList, CB_GETCURSEL, 0, 0);
                                    SendMessage(hList, CB_GETLBTEXT, index, (LPARAM) g_tcsTemp);
                                    GetDateFormat(g_iCurLocale, 0, NULL, g_tcsTemp, tcTemp, MAX_STR);
                                    SetWindowText(GetDlgItem(hDlg, IDC_SDATESAMPLE), tcTemp);
                                }
                            }
                            break;

                        case IDC_LDATEFORMAT:
                            {
                                if(HIWORD(wParam) == CBN_SELCHANGE)
                                {
                                    // long date formating selection has been changed.
                                    // Re-edit long date
                                    HWND    hList = NULL;
                                    LRESULT index = 0;
                                    TCHAR    tcTemp[MAX_STR];

                                    hList = GetDlgItem(hDlg, IDC_LDATEFORMAT);
                                    
                                    if(NULL == hList)
                                    {
                                        bProcessedMsg = FALSE;
                                        break;
                                    }

                                    index = SendMessage(hList, CB_GETCURSEL, 0, 0);
                                    SendMessage(hList, CB_GETLBTEXT, index, (LPARAM) g_tcsTemp);
                                    GetDateFormat(g_iCurLocale, 0, NULL, g_tcsTemp, tcTemp, MAX_STR);
                                    SetWindowText(GetDlgItem(hDlg, IDC_LDATESAMPLE), tcTemp);
                                }
                            }
                            break;
                    }
                    break;

        default:
            bProcessedMsg = FALSE;
    }
    
    return bProcessedMsg;
}


///////////////////////////////////////////////////////////////////////////////////////////
//  Function:       EnumLocalesProc
//
//  Description:    Call back function for supported locale enumeration.
//                  In this callback function we retrieve different information
//                  related to the enumerated locale and append that info to our listview
//                  control.
//
//  Comments:
//
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK EnumLocalesProc(LPTSTR lpLocaleString)
{
    int     iCurLocale = 0;
    TCHAR   szCurName[STR_LEN];
    LVITEM  lviItem = {0};
    int     iNewItem  = 0;
    BOOL    bProcessed = TRUE;

    if (!lpLocaleString)
    {
        bProcessed = FALSE;
    }
    else
    {
        iCurLocale = iConvertStrToInt(lpLocaleString);

        // initialize out LV item
        lviItem.mask = LVIF_TEXT | LVIF_PARAM;    
        lviItem.cchTextMax  = STR_LEN;
        lviItem.lParam = g_iCounter ;

        // Get LangID and add it in LV
        GetLocaleInfo(iCurLocale, LOCALE_ILANGUAGE , szCurName, STR_LEN);
        lviItem.pszText = szCurName;
        iNewItem = ListView_InsertItem(g_hwndList, &lviItem);

        // Get and write localize language name
        GetLocaleInfo(iCurLocale, LOCALE_SLANGUAGE , szCurName, STR_LEN);
        ListView_SetItemText(g_hwndList, iNewItem, 1, szCurName);

        // Get and write Native Language Name...
        GetLocaleInfo(iCurLocale, LOCALE_SNATIVELANGNAME , szCurName, STR_LEN);
        ListView_SetItemText(g_hwndList, iNewItem, 2, szCurName);

        // Get and write Native Country Name...
        GetLocaleInfo(iCurLocale, LOCALE_SNATIVECTRYNAME , szCurName, STR_LEN);
        ListView_SetItemText(g_hwndList, iNewItem, 3, szCurName);

        // Get and write ANSI CP.
        GetLocaleInfo(iCurLocale, LOCALE_IDEFAULTANSICODEPAGE , szCurName, STR_LEN);
        ListView_SetItemText(g_hwndList, iNewItem, 4, szCurName);

        // Get and write OEM CP.
        GetLocaleInfo(iCurLocale, LOCALE_IDEFAULTCODEPAGE , szCurName, STR_LEN);
        ListView_SetItemText(g_hwndList, iNewItem, 5, szCurName);

        g_iCounter ++;
    }

    return bProcessed;
}

///////////////////////////////////////////////////////////////////////////////////////////
//  Function:       iConvertStrToInt
//
//  Description:    Converts a hex value saved as a string (coming from EnumSystemLocale)
//                  to an integer.
//
//  Comments:
//
///////////////////////////////////////////////////////////////////////////////////////////
int iConvertStrToInt(LPTSTR strString)
{
int		i, iNum;
int		iValue = 0;
int		iboundry;

	// convert the string to upper case first.
	_tcsupr(strString);

	// we look into 4 LSB only since:
	// LCID (DWORD) = Sort-Key (HI-WORD) + Lang ID (LOW-WORD)
    __try
    {
        // If strString is not null terminated , this call can cause access violation.
        iboundry = _tcslen(strString) - 1;
    }

    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        iboundry = 0;
    }


	for (i = iboundry; i > iboundry - 4; i--)
	{
		iNum = strString[i] - 48;	// 48 being the ASCII number for 0
		if (iNum > 10)				// if it's over digit range
			iNum -= 7;
		iValue += iNum * (int)(pow(16, iboundry-i));
	}
	return iValue;
}

///////////////////////////////////////////////////////////////////////////////////////////
//  Function:       InitNLSFields
//
//  Description:    Initialize NLS formatting fields for a given LCID
//
//  Comments:
//
///////////////////////////////////////////////////////////////////////////////////////////
void InitNLSFields(HWND hDlg, LCID lcid)
{
    TCHAR   tcsTemp[MAX_STR];
    HWND    hList = NULL;

    // Init larg number fields...
    LoadString(g_hInst, STR_LARGEPOSNUMBER, g_tcsTemp, MAX_STR);
    GetNumberFormat(lcid, 0, g_tcsTemp, NULL, tcsTemp, MAX_STR);
    SetWindowText(GetDlgItem(hDlg, IDC_POS_NUMBER), tcsTemp);

    LoadString(g_hInst, STR_LARGENEGNUMBER, g_tcsTemp, MAX_STR);
    GetNumberFormat(lcid, 0, g_tcsTemp, NULL, tcsTemp, MAX_STR);
    SetWindowText(GetDlgItem(hDlg, IDC_NEG_NUMBER), tcsTemp);

    // Init the currency field format...
    LoadString(g_hInst, STR_LARGEPOSNUMBER, g_tcsTemp, MAX_STR);
    GetCurrencyFormat(lcid, 0, g_tcsTemp, NULL, tcsTemp, MAX_STR);
    SetWindowText(GetDlgItem(hDlg, IDC_POS_CURRENCY), tcsTemp);

    LoadString(g_hInst, STR_LARGENEGNUMBER, g_tcsTemp, MAX_STR);
    GetCurrencyFormat(lcid, 0, g_tcsTemp, NULL, tcsTemp, MAX_STR);
    SetWindowText(GetDlgItem(hDlg, IDC_NEG_CURRENCY), tcsTemp);

    // Init time field format...
    // Delete our previous list of items...
    hList = GetDlgItem(g_hDlg, IDC_TIMEFORMAT);
    if(NULL != hList)
    {
        SendMessage(hList, CB_RESETCONTENT , 0, 0);

        // Enumerates the time formats that are available for a specified locale.
        EnumTimeFormats(EnumTimeFormatsProc, lcid, 0);
        SendMessage(hList, CB_SETCURSEL, 0, 0);          // item index
        SendMessage(hList, CB_GETLBTEXT, 0, (LPARAM) g_tcsTemp);
        GetTimeFormat(lcid, 0, NULL, g_tcsTemp, tcsTemp, MAX_STR);
        SetWindowText(GetDlgItem(hDlg, IDC_TIMESAMPLE), tcsTemp);
    }

    // Init calendar field format...
    // Delete our previous list of items...
    hList = GetDlgItem(g_hDlg, IDC_CALFORMAT);
    if(NULL != hList)
    {
        SendMessage(hList, CB_RESETCONTENT , 0, 0);

        // Enumerates the short date formats that are available for a specified locale.
        EnumCalendarInfo(EnumCalendarInfoProc, lcid, ENUM_ALL_CALENDARS, CAL_SCALNAME);
        SendMessage(hList, CB_SETCURSEL, 0, 0);          // item index
        SendMessage(hList, CB_GETLBTEXT, 0, (LPARAM) g_tcsTemp);
    }

    // Init short date field format...
    // Delete our previous list of items...
    hList = GetDlgItem(g_hDlg, IDC_SDATEFORMAT);
    if(NULL != hList)
    {
        SendMessage(hList, CB_RESETCONTENT , 0, 0);

        // Enumerates the short date formats that are available for a specified locale.
        EnumDateFormats(EnumSDateFormatsProc, lcid, DATE_SHORTDATE);
        SendMessage(hList, CB_SETCURSEL, 0, 0);          // item index
        SendMessage(hList, CB_GETLBTEXT, 0, (LPARAM) g_tcsTemp);
        GetDateFormat(lcid, 0, NULL, g_tcsTemp, tcsTemp, MAX_STR);
        SetWindowText(GetDlgItem(hDlg, IDC_SDATESAMPLE), tcsTemp);
    }

    // Init long date field format...
    // Delete our previous list of items...
    hList = GetDlgItem(g_hDlg, IDC_LDATEFORMAT);
    if(NULL != hList)
    {
        SendMessage(hList, CB_RESETCONTENT , 0, 0);

        // Enumerates the long date formats that are available for a specified locale
        EnumDateFormats(EnumLDateFormatsProc, lcid, DATE_LONGDATE);
        SendMessage(hList, CB_SETCURSEL, 0, 0);          // item index
        SendMessage(hList, CB_GETLBTEXT, 0, (LPARAM) g_tcsTemp);
        GetDateFormat(lcid, 0, NULL, g_tcsTemp, tcsTemp, MAX_STR);
        SetWindowText(GetDlgItem(hDlg, IDC_LDATESAMPLE), tcsTemp);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
//  Function:       EnumTimeFormatsProc
//
//  Description:    Callback function for time format enumeration.
//
//  Comments:
//
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK EnumTimeFormatsProc(LPTSTR lpTimeFormatString)
{
    BOOL bProcessed = TRUE;

    if (!lpTimeFormatString)
    {
        bProcessed = FALSE;
    }
    else
    {
        // Fill into the combo box the list of supported time formats...
        SendMessage(GetDlgItem(g_hDlg, IDC_TIMEFORMAT), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)lpTimeFormatString);            
    }

    return bProcessed;
}


///////////////////////////////////////////////////////////////////////////////////////////
//  Function:   EnumSDateFormatsProc
//
//  Description:    Callback function for short date format enumeration.
//
//  Comments:
//
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK EnumSDateFormatsProc(LPTSTR lpDateFormatString)
{
    BOOL bProcessed = TRUE;
    if (!lpDateFormatString)
    {
        return FALSE;
    }
    else
    {
        // Fill into the combo box the list of supported short date formats...
        SendMessage(GetDlgItem(g_hDlg, IDC_SDATEFORMAT), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)lpDateFormatString);
    }

    return bProcessed;
}

///////////////////////////////////////////////////////////////////////////////////////////
//  Function:       EnumLDateFormatsProc
//
//  Description:    Callback function for long date format enumeration.
//
//  Comments:
//
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK EnumLDateFormatsProc(LPTSTR lpDateFormatString)
{
    BOOL bProcessed = TRUE;
    
    if (!lpDateFormatString)
    {
        return FALSE;
    }
    else
    {
        // Fill into the combo box the list of supported long date formats...
        SendMessage(GetDlgItem(g_hDlg, IDC_LDATEFORMAT), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)lpDateFormatString);
    }

    return bProcessed;
}

///////////////////////////////////////////////////////////////////////////////////////////
//  Function:       EnumCalendarInfoProc
//
//  Description:    Callback function for calendar name enumeration.
//
//  Comments:   
//
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK EnumCalendarInfoProc(LPTSTR lpCalendarInfoString)
{
    BOOL bProcessed = TRUE;

    if (!lpCalendarInfoString)
    {
        return FALSE;
    }
    else
    {
        // Fill into the combo box the list of supported calendars...
        SendMessage(GetDlgItem(g_hDlg, IDC_CALFORMAT), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)lpCalendarInfoString);
    }

    return bProcessed;
}


///////////////////////////////////////////////////////////////////////////////////////////
//  Function:       LcidDlgProc
//
//  Description:    Message-processing function for lcid windows.
//
//  Comments:
//
///////////////////////////////////////////////////////////////////////////////////////////
INT_PTR CALLBACK LcidDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL bProcessedMsg = TRUE;
    
    switch(uMsg)
    {
    case WM_INITDIALOG:
        {
            TCHAR   szCurName[STR_LEN];

            // Get and write Localized Name...
            GetLocaleInfo(g_iCurLocale, LOCALE_SLANGUAGE , szCurName, STR_LEN);
            SetWindowText(GetDlgItem(hDlg, IDC_LANGNAME), szCurName);

            // Get and write Native Language Name...
            GetLocaleInfo(g_iCurLocale, LOCALE_SNATIVELANGNAME , szCurName, STR_LEN);
            SetWindowText(GetDlgItem(hDlg, IDC_NLANGNAME), szCurName);

            // Get and write Native Country Name...
            GetLocaleInfo(g_iCurLocale, LOCALE_SNATIVECTRYNAME , szCurName, STR_LEN);
            SetWindowText(GetDlgItem(hDlg, IDC_NCOUNTRYREGIONNAME), szCurName);

            // Get and write ANSI CP
            GetLocaleInfo(g_iCurLocale, LOCALE_IDEFAULTANSICODEPAGE , szCurName, STR_LEN);
            SetWindowText(GetDlgItem(hDlg, IDC_ACP), szCurName);

            // Get and write OEM CP
            GetLocaleInfo(g_iCurLocale, LOCALE_IDEFAULTCODEPAGE , szCurName, STR_LEN);
            SetWindowText(GetDlgItem(hDlg, IDC_OEM), szCurName);
        }
        break;

    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
            case IDOK:      /* fall-through */
            case IDCANCEL:
                bProcessedMsg = EndDialog(hDlg, TRUE);
                break;
        }
     break;

    default:
        bProcessedMsg = FALSE;
        break;
    }

    return bProcessedMsg;
}

///////////////////////////////////////////////////////////////////////////////////////////
//  Function:       CompareFunc
//
//  Description:    An enumeration function for our list view control
//                  lParam1: the lparam parameter of the first item to be compared
//                  lParam2: the lparam of the second item we are comparing against
//                  lParamSort: the column to which the items  to be compared belongs to
//
//  Comments:
//
///////////////////////////////////////////////////////////////////////////////////////////
int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
int     iRet = 0;
TCHAR   csItem1[STR_LEN], csItem2[STR_LEN];

    // We are just reversing the order, just leave now.
    if (lParamSort == -1)
    {
        iRet = 1;
    }
    else
    {
        // find the text of the subitem that we want to sort...
        ListView_GetItemText(g_hwndList, (int)lParam1, (int)lParamSort, csItem1, STR_LEN);
        ListView_GetItemText(g_hwndList, (int)lParam2, (int)lParamSort, csItem2, STR_LEN);

        switch (lParamSort)
        {
            case COLUMN_LANGID:
                iRet = lstrcmp(csItem1, csItem2);
                break;

            case COLUMN_LANGUAGE:
                iRet = lstrcmp(csItem1, csItem2);
                break;

            case COLUMN_ACP:
                iRet = _ttoi(csItem1) - _ttoi (csItem2);
                break;

        case COLUMN_OEMCP:
                iRet = _ttoi(csItem1) - _ttoi (csItem2);
                break;

            // Don't do anything. We should not get here.
            default:
                iRet = -1;
        }
    }

    return iRet;
}
