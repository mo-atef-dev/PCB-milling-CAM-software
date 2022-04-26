#include "app.h"

BOOL CALLBACK DlgProc_MaxCopper(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static DlgStrct_MaxCopper* pResult;
    switch (message)
    {
    case WM_INITDIALOG:
        pResult = (DlgStrct_MaxCopper*)lParam;
        printf("lParam : %x\n", (lParam));
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            char lpString[10];
            pResult->valid = true;
            printf("Check Point 1\n");

            GetDlgItemText(hwndDlg, IDC_EDIT_PTOM, lpString, 10);
            // Check that the string has only positive characters and a single decimal point
            for(int i = 0; i < 10 && lpString[i] != 0; i++)
            {
                if(lpString[i] < '0' || lpString[i] > '9')
                {
                    if(lpString[i] != '.')
                    {
                        pResult->valid = false;
                    }
                }
            }
            printf("Check Point 2\n");

            pResult->pixTomm = atof(lpString);

            GetDlgItemText(hwndDlg, IDC_EDIT_ZTOP, lpString, 10);
            // Check that the string has only positive characters and a single decimal point
            for(int i = 0; i < 10 && lpString[i] != 0; i++)
            {
                if(lpString[i] < '0' || lpString[i] > '9')
                {
                    if(lpString[i] != '-')
                    {
                        pResult->valid = false;
                    }
                }
            }
            printf("Check Point 3\n");

            pResult->zTop = atoi(lpString);

            GetDlgItemText(hwndDlg, IDC_EDIT_ZBOT, lpString, 10);
            // Check that the string has only positive characters and a single decimal point
            for(int i = 0; i < 10 && lpString[i] != 0; i++)
            {
                if(lpString[i] < '0' || lpString[i] > '9')
                {
                    if(lpString[i] != '-')
                    {
                        pResult->valid = false;
                    }
                }
            }
            printf("Check Point 4\n");

            pResult->zBottom = atoi(lpString);

        // Fall through.

        case IDCANCEL:
            EndDialog(hwndDlg, wParam);
            return TRUE;
        }
    }
    return FALSE;
}
