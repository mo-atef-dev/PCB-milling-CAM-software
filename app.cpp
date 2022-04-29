#include "app.h"

static TCHAR szChildClassName[ ] = _T("LayerWindow");

int App_Initialize(HINSTANCE hThisInstance)
{
    WNDCLASSEX childcl;

    /* The child window structure */
    childcl.hInstance = hThisInstance;
    childcl.lpszClassName = szChildClassName;
    childcl.lpfnWndProc = WinProc_Layers;
    childcl.style = CS_DBLCLKS;
    childcl.cbSize = sizeof(WNDCLASSEX);

    childcl.hIcon = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));
    childcl.hIconSm = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));
    childcl.hCursor = LoadCursor (NULL, IDC_ARROW);
    childcl.lpszMenuName = MAKEINTRESOURCE(RSRC_MENU);                 /* No menu */
    childcl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    childcl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    childcl.hbrBackground = (HBRUSH) COLOR_WINDOW;

    if(!RegisterClassEx(&childcl))
    {
        MessageBox(NULL, "Failed to register layers child window class", "ERROR", MB_OK | MB_ICONERROR);
        return 1;
    }
    return 0;
}
HWND App_CreateLayersWindow(HWND hwnd)
{
    HWND cHwnd;
    cHwnd = CreateWindowEx(0,
                           szChildClassName,
                           _T("Files"),
                           WS_CHILD | WS_BORDER,
                           100, 100, 100, 100,
                           hwnd,
                           (HMENU) ID_CHILD_Layers,
                           GetModuleHandle(NULL),
                           NULL);

    if(!cHwnd)
    {
        MessageBox(NULL, "Failed to show layers child window class", "ERROR", MB_OK | MB_ICONERROR);
        return NULL;
    }
    ShowWindow(cHwnd, SW_SHOW);
    return cHwnd;
}

int App_ResizeLayersWindow(HWND hwndParent)
{

}

BOOL CALLBACK DlgProc_MaxCopper(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static DlgStrct_MaxCopper* pResult;
    switch (message)
    {
    case WM_INITDIALOG:
        pResult = (DlgStrct_MaxCopper*)lParam;
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_OK:
            char lpString[10];
            pResult->valid = true;

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

            pResult->zBottom = atoi(lpString);

        // Fall through.

        case ID_CANCEL:
            EndDialog(hwndDlg, wParam);
            return TRUE;
        }
    }
    return FALSE;
}

LRESULT CALLBACK WinProc_Layers(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc (hwnd, message, wParam, lParam);
}

int App_OpenFile(HWND hwnd)
{
    // Process the open file command
    #warning "The constant bellow for file title is a place holder and should be replaced"
    OPENFILENAMEW ofn;       // common dialog box structure
    WCHAR szFile[MAX_PATH];      // buffer for file name
    WCHAR szFileTitle[256];

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0] = L'\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All\0*.*\0Text (.txt)\0*.TXT\0Gerber (.gbr, .gb)\0*.gbr;*.gb\0Image (.bmp)\0*.bmp\0"; // The file type filters
    ofn.nFilterIndex = 3; // Index of default filter (starting from 1), in this case 3 refers to Gerber files
    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFileTitle = 10;
    ofn.lpstrInitialDir = L"Documents";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

    printf("\nStarting open file dialog box\n");

    // Display the Open dialog box.
    if (GetOpenFileNameW(&ofn)==TRUE)
    {
        wcscpy_s(szGerberPath, MAX_PATH, szFile);
        MessageBoxW(hwnd, szGerberPath, L"File Opened Successfully", MB_OK | MB_ICONINFORMATION);
        // Open file to get information
        HANDLE hfile = CreateFileW(szGerberPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if(hfile)
        {
            DWORD fileSize = GetFileSize(hfile, NULL);
            if(szGerberBuffer != NULL)
                delete[] szGerberBuffer;
            szGerberBuffer = new CHAR[fileSize+1];    // Extra character to insert a 0 in case the file chosen is not null terminated
            DWORD bytesRead = 0;
            if(ReadFile(hfile, szGerberBuffer, fileSize, &bytesRead, NULL))
            {
                printf("\nFile read successfully\n");
                if(bytesRead > fileSize)
                {
                    printf("\nERROR: Number of bytes read is more than the maximum allowed!\n");
                }
                szGerberBuffer[fileSize]= 0;  // Safe null terminator
                EnableMenuItem(menu, ID_MAXCPR, MF_ENABLED);
            }
            else
            {
                MessageBox(NULL, "Failed to read file", "Error", MB_OK | MB_ICONERROR);
                delete[] szGerberBuffer;
            }
        }
        else
        {
            MessageBox(NULL, "Failed to create file handle", "Error", MB_OK | MB_ICONERROR);
        }

        // Invalidate window to force it to completely redraw
        InvalidateRect(hwnd, NULL, FALSE);
    }
    else
    {
        MessageBox(NULL, "Failed to open file", "Error", MB_OK | MB_ICONERROR);
    }
}
