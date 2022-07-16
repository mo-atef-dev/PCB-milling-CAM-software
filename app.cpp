#include "app.h"

// Debug related header includes
#if defined(DEBUG)
#include <stdio.h>
#else
#define printf(...)
#define wprintf(...)
#endif // defined

/* Class names */
static TCHAR szLayersClassName[ ] = _T("LayerWindow");
static TCHAR szDrawClassName[ ] = _T("DrawWindow");

/* Global variables for drawing window coordinates */
FLOAT origin_x = 0;
FLOAT origin_y = 0;
FLOAT img_scale = 1;

/* Global variable to determine drawing mode */
char disp = 0;

int App_Initialize(HINSTANCE hThisInstance)
{
    WNDCLASSEX drawCl;
    WNDCLASSEX layerCl;

    /* The child window structure */
    layerCl.hInstance = hThisInstance;
    layerCl.lpszClassName = szLayersClassName;
    layerCl.lpfnWndProc = WinProc_Layers;
    layerCl.style = CS_DBLCLKS;
    layerCl.cbSize = sizeof(WNDCLASSEX);

    layerCl.hIcon = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));
    layerCl.hIconSm = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));
    layerCl.hCursor = LoadCursor (NULL, IDC_ARROW);
    layerCl.lpszMenuName = MAKEINTRESOURCE(RSRC_MENU);                 /* No menu */
    layerCl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    layerCl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    layerCl.hbrBackground = (HBRUSH) COLOR_WINDOW;

    if(!RegisterClassEx(&layerCl))
    {
        MessageBox(NULL, "Failed to register layers child window class", "ERROR", MB_OK | MB_ICONERROR);
        return 1;
    }

     /* The child window structure */
    drawCl.hInstance = hThisInstance;
    drawCl.lpszClassName = szDrawClassName;
    drawCl.lpfnWndProc = WinProc_Draw;
    drawCl.style = CS_DBLCLKS;
    drawCl.cbSize = sizeof(WNDCLASSEX);

    drawCl.hIcon = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));
    drawCl.hIconSm = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));
    drawCl.hCursor = LoadCursor (NULL, IDC_CROSS);
    drawCl.lpszMenuName = MAKEINTRESOURCE(RSRC_MENU);                 /* No menu */
    drawCl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    drawCl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    drawCl.hbrBackground = CreateSolidBrush(RGB(200, 200, 200));

    if(!RegisterClassEx(&drawCl))
    {
        MessageBox(NULL, "Failed to register drawing child window class", "ERROR", MB_OK | MB_ICONERROR);
        return 1;
    }

    return 0;
}

HWND App_CreateLayersWindow(HWND hwnd)
{
    HWND cHwnd;
    cHwnd = CreateWindowEx(0,
                           szLayersClassName,
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

    // Init drawing module
    initializeDrawing(cHwnd);

    ShowWindow(cHwnd, SW_SHOW);
    return cHwnd;
}

HWND App_CreateDrawingWindow(HWND hwnd)
{
    HWND cHwnd;
    cHwnd = CreateWindowEx(0,
                           szDrawClassName,
                           _T("Files"),
                           WS_CHILD | WS_BORDER,
                           100, 100, 100, 100,
                           hwnd,
                           (HMENU) ID_CHILD_Draw,
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

LRESULT CALLBACK WinProc_Draw(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //static CMNStatusBar status_bar;
    static bool RBDown = false;
    static bool LBDown = false;

    static DWORD init_mouse_x = 0;
    static DWORD init_mouse_y = 0;

    switch(message)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            scaleResize(hwnd,pRT,img_scale,origin_x,origin_y,D2DpBitmap);

            EndPaint(hwnd, &ps);
        }
        break;
        case WM_LBUTTONDOWN:
        {
            LBDown = true;
            init_mouse_x = GET_X_LPARAM(lParam);
            init_mouse_y = GET_Y_LPARAM(lParam);
            printf("\nLeft mouse clicked\n");
        }
        break;
        case WM_LBUTTONUP:
        {
            LBDown = false;
            printf("\nLeft mouse clicked\n");
        }
        break;
        case WM_MOUSEMOVE:
        {
            char xstr[20];
            char ystr[20];
            if(LBDown)
            {
                // Split into to assignments to avoid doing unsigned arithmetic
                origin_x += GET_X_LPARAM(lParam);
                origin_x -= init_mouse_x;
                origin_y += GET_Y_LPARAM(lParam);
                origin_y -= init_mouse_y;
                printf("## Left Button is held with delta (%d, %d) ##\n", GET_X_LPARAM(lParam) - init_mouse_x, GET_Y_LPARAM(lParam) - init_mouse_y);
                printf("Origin = (%f, %f)\n", origin_x, origin_y);
                init_mouse_x = GET_X_LPARAM(lParam);
                init_mouse_y = GET_Y_LPARAM(lParam);
                InvalidateRect(hwnd, NULL, FALSE);
            }
            sprintf_s(xstr, 20, "Center X : %.1f", (-origin_x));
            sprintf_s(ystr, 20, "Center Y : %.1f", (-origin_y));
            status_bar.UpdateText(1, xstr);
            status_bar.UpdateText(2, ystr);
        }
        break;
        case WM_MOUSEWHEEL:
        {
            // To preserve small wheel increments across different messages
            static int delta = 0;
            char zoomstr[20];
            delta += GET_WHEEL_DELTA_WPARAM(wParam);

            if(delta >= WHEEL_DELTA)
            {
                img_scale *= ZOOMIN_FAC;
                delta = 0;
                InvalidateRect(hwnd, NULL, FALSE);
            }
            else if(delta <= -WHEEL_DELTA)
            {
                img_scale *= ZOOMOUT_FAC;
                delta = 0;
                InvalidateRect(hwnd, NULL, FALSE);
            }
            sprintf_s(zoomstr, 20, "Zoom : %.1f%%", img_scale*100.0);
            status_bar.UpdateText(4, zoomstr);
        }
        break;
    case WM_RBUTTONDOWN:
        if(RBDown) break;
        RBDown = true;
        if(disp == 0)
            DrawBitmapOnWindow(hwnd,pFactory_, pBitmap_2,wicFactory_);
        else if(disp == 1)
            DrawBitmapOnWindow(hwnd,pFactory_, pBitmap_,wicFactory_);
        else if(disp == 2)
            DrawBitmapOnWindow(hwnd,pFactory_, pBitmap_3,wicFactory_);
        disp = (disp+1)%3;
        break;

    case WM_RBUTTONUP:
        RBDown = false;
        break;
    default:
        return DefWindowProc (hwnd, message, wParam, lParam);
    }
}

BOOL CALLBACK DlgProc_MaxCopper(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static DlgStrct_MaxCopper* pResult;
    switch (message)
    {
    case WM_INITDIALOG:
        pResult = (DlgStrct_MaxCopper*)lParam;

        // Set the default values of the dialog
        SetDlgItemText(hwndDlg, IDC_EDIT_PTOM, "0.1");
        SetDlgItemText(hwndDlg, IDC_EDIT_ZTOP, "2");
        SetDlgItemText(hwndDlg, IDC_EDIT_ZBOT, "-0.25");

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
            // Check that the string has only digits, minus sign, and decimal points
            for(int i = 0; i < 10 && lpString[i] != 0; i++)
            {
                if(lpString[i] < '0' || lpString[i] > '9')
                {
                    if(lpString[i] != '-' && lpString[i] != '.')
                    {
                        pResult->valid = false;
                    }
                }
            }

            pResult->zTop = (atof(lpString)) / pResult->pixTomm;

            GetDlgItemText(hwndDlg, IDC_EDIT_ZBOT, lpString, 10);
            // Check that the string has only digits, minus sign, and decimal points
            for(int i = 0; i < 10 && lpString[i] != 0; i++)
            {
                if(lpString[i] < '0' || lpString[i] > '9')
                {
                    if(lpString[i] != '-' && lpString[i] != '.')
                    {
                        pResult->valid = false;
                    }
                }
            }

            pResult->zBottom = (atof(lpString)) / pResult->pixTomm;

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
    static CMNTextBox txtBox_copper;
    static CMNTextBox txtBox_border;
    static CMNTextBox txtBox_drills;
    static CMNTextBox txtBox_trace;
    static CMNTextBox txtBox_cmds;
    static CMNTextBox txtBox_mmg;

    static CMNLabel label_copper;
    static CMNLabel label_border;
    static CMNLabel label_drills;
    static CMNLabel label_trace;
    static CMNLabel label_cmds;
    static CMNLabel label_mmg;

    //static CMNButton btn_copper;
    //static CMNButton btn_border;
    //static CMNButton btn_drills;
    //static CMNButton btn_trace;
    //static CMNButton btn_cmds;

    switch(message)
    {
    case WM_CREATE:
        {
            // Create the controls of the window here
            // Most of this code is hard-coded layouting code which is not optimal

            int yoffset = layersPadding;

            label_copper.InitLabel(hwnd, 0, GetModuleHandle(NULL), layersPadding, yoffset, layersWidth-2*layersPadding, 20, WS_CHILD | WS_VISIBLE, "Gerber Copper File");
            txtBox_copper.InitBox(hwnd, 0, GetModuleHandle(NULL), layersPadding, yoffset + layersItemsSpacing/4 + 20, layersWidth-4*layersPadding, 20, CMNTXT_READONLY);
            //btn_copper.InitButton(hwnd, 0, GetModuleHandle(NULL), layersWidth - layersItemsSpacing*4, yoffset + layersItemsSpacing/4 + 20, layersItemsSpacing*4-layersPadding, 20, BS_DEFPUSHBUTTON | WS_CHILD | WS_VISIBLE, "Browse");

            yoffset = yoffset + layersItemsSpacing/4 + 20 + layersItemsSpacing*2;
            label_border.InitLabel(hwnd, 0, GetModuleHandle(NULL), layersPadding, yoffset, layersWidth-2*layersPadding, 20, WS_CHILD | WS_VISIBLE, "Gerber Border File");
            txtBox_border.InitBox(hwnd, 0, GetModuleHandle(NULL), layersPadding, yoffset + layersItemsSpacing/4 + 20, layersWidth-4*layersPadding, 20, CMNTXT_READONLY);
            //btn_border.InitButton(hwnd, 0, GetModuleHandle(NULL), layersWidth - layersItemsSpacing*4, yoffset + layersItemsSpacing/4 + 20, layersItemsSpacing*4-layersPadding, 20, BS_DEFPUSHBUTTON | WS_CHILD | WS_VISIBLE, "Browse");

            yoffset = yoffset + layersItemsSpacing/4 + 20 + layersItemsSpacing*2;
            label_drills.InitLabel(hwnd, 0, GetModuleHandle(NULL), layersPadding, yoffset, layersWidth-2*layersPadding, 20, WS_CHILD | WS_VISIBLE, "Gerber Drill File");
            txtBox_drills.InitBox(hwnd, 0, GetModuleHandle(NULL), layersPadding, yoffset + layersItemsSpacing/4 + 20, layersWidth-4*layersPadding, 20, CMNTXT_READONLY);
            //btn_drills.InitButton(hwnd, 0, GetModuleHandle(NULL), layersWidth - layersItemsSpacing*4, yoffset + layersItemsSpacing/4 + 20, layersItemsSpacing*4-layersPadding, 20, BS_DEFPUSHBUTTON | WS_CHILD | WS_VISIBLE, "Browse");

            yoffset = yoffset + layersItemsSpacing/4 + 20 + layersItemsSpacing*2;
            label_trace.InitLabel(hwnd, 0, GetModuleHandle(NULL), layersPadding, yoffset, layersWidth-2*layersPadding, 20, WS_CHILD | WS_VISIBLE, "Isolation Image File");
            txtBox_trace.InitBox(hwnd, 0, GetModuleHandle(NULL), layersPadding, yoffset + layersItemsSpacing/4 + 20, layersWidth-4*layersPadding, 20, CMNTXT_READONLY);
            //btn_trace.InitButton(hwnd, 0, GetModuleHandle(NULL), layersWidth - layersItemsSpacing*4, yoffset + layersItemsSpacing/4 + 20, layersItemsSpacing*4-layersPadding, 20, BS_DEFPUSHBUTTON | WS_CHILD | WS_VISIBLE, "Browse");

            yoffset = yoffset + layersItemsSpacing/4 + 20 + layersItemsSpacing*2;
            label_mmg.InitLabel(hwnd, 0, GetModuleHandle(NULL), layersPadding, yoffset, layersWidth-2*layersPadding, 20, WS_CHILD | WS_VISIBLE, "MMG File");
            txtBox_mmg.InitBox(hwnd, 0, GetModuleHandle(NULL), layersPadding, yoffset + layersItemsSpacing/4 + 20, layersWidth-4*layersPadding, 20, CMNTXT_READONLY);

            yoffset = yoffset + layersItemsSpacing/4 + 20 + layersItemsSpacing*2;
            label_cmds.InitLabel(hwnd, 0, GetModuleHandle(NULL), layersPadding, yoffset, layersWidth-2*layersPadding, 20, WS_CHILD | WS_VISIBLE, "Generated Commands File");
            txtBox_cmds.InitBox(hwnd, 0, GetModuleHandle(NULL), layersPadding, yoffset + layersItemsSpacing/4 + 20, layersWidth-4*layersPadding, 20, CMNTXT_READONLY);
            //btn_cmds.InitButton(hwnd, 0, GetModuleHandle(NULL), layersWidth - layersItemsSpacing*4, yoffset + layersItemsSpacing/4 + 20, layersItemsSpacing*4-layersPadding, 20, BS_DEFPUSHBUTTON | WS_CHILD | WS_VISIBLE, "Browse");
        }
        break;
    case WMU_UPDATE:
        if(szGerberPath)
            txtBox_copper.SetText(szGerberPath, MAX_PATH);

        if(szBorderPath)
            txtBox_border.SetText(szBorderPath, MAX_PATH);

        if(szDrillPath)
            txtBox_drills.SetText(szDrillPath, MAX_PATH);

        if(szTracePath)
            txtBox_trace.SetText(szTracePath, MAX_PATH);

        if(szMMGPath)
            txtBox_mmg.SetText(szMMGPath, MAX_PATH);

        if(szCmdsPath)
            txtBox_cmds.SetText(szCmdsPath, MAX_PATH);
        break;
    default:
        return DefWindowProc (hwnd, message, wParam, lParam);
    }
}

//string App_GenerateMMGFromImage(bitmap_image* bitmap, const int zTop, const int zBottom, const double pixTomm = 0.1f);
//string App_GenerateMMGFromImage(const MyBitmap& bitmap, const int zTop, const int zBottom, const double pixTomm = 0.1f);
//vector<OutCommand> App_GenerateCmdsFromMMG(const string& mmg);
//void App_ExportMMGFile(const string& mmg, CHAR* file);
//void App_ExportCmds(const vector<OutCommand>& cmds, CHAR* file);

int App_OpenGbrFile(HWND hwnd)
{
    int r = App_OpenFile(hwnd, szGerberPath, szGerberBuffer, true, L"Gerber (.gbr, .gb)\0*.gbr;*.gb\0", 1);
    if(r == 0) EnableMenuItem(menu, ID_MAXCPR, MF_ENABLED);
    return r;
}

int App_OpenDrillFile(HWND hwnd)
{
    int r = App_OpenFile(hwnd, szDrillPath, szDrillBuffer, true, L"Drill (.drl, .xln)\0*.drl;*.xln\0", 1);
    if(r == 0) EnableMenuItem(menu, ID_MAXCPR, MF_ENABLED);
    return r;
}

int App_OpenBorderFile(HWND hwnd)
{
    int r = App_OpenFile(hwnd, szBorderPath, szBorderBuffer, true, L"Gerber (.gbr, .gb)\0*.gbr;*.gb\0", 1);
    if(r == 0) EnableMenuItem(menu, ID_MAXCPR, MF_ENABLED);
    return r;
}

int App_OpenTraceImage(HWND hwnd)
{
    int r = App_OpenFile(hwnd, szTracePath, szGerberBuffer, false, L"Image (.bmp)\0*.bmp\0", 1);
    if(pTraceImage != NULL)
        delete pTraceImage;
    std::wstring w_str(szTracePath);
    pTraceImage = new bitmap_image(string(w_str.begin(), w_str.end()));
    if(r == 0) EnableMenuItem(menu, ID_MMG, MF_ENABLED);
    return r;
}

int App_OpenCopperImage(HWND hwnd)
{
    App_OpenFile(hwnd, szGerberPath, szGerberBuffer, true, L"All\0*.*\0Text (.txt)\0*.TXT\0Gerber (.gbr, .gb)\0*.gbr;*.gb\0Image (.bmp)\0*.bmp\0", 3);
}

int App_OpenMMGFile(HWND hwnd)
{
    CHAR* szTempBuffer = NULL;
    int r = App_OpenFile(hwnd, szMMGPath, szTempBuffer, true, L"All\0*.*\0Model Master 3 Axis (.mmg)\0*.mmg\0", 1);
    string tempStr(szTempBuffer);
    mmgString = tempStr;
    delete[] szTempBuffer;
    szTempBuffer = NULL;
    if(r == 0) EnableMenuItem(menu, ID_CMDS, MF_ENABLED);
    return r;
}

int App_OpenCommands(HWND hwnd)
{
    int r = App_OpenFile(hwnd, szCmdsPath, szGerberBuffer, false, L"All\0*.*\0Binary (.hex, .dat, .bin)\0*.hex;*.bin;*.dat\0", 1);
    return r;
}

int App_OpenFile(HWND hwnd, WCHAR* szFilePath, CHAR* &FileBuffer, BOOL LoadFile, const LPCWSTR lpstrFilter, DWORD nFilterIndex)
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
    ofn.lpstrFilter = lpstrFilter; // The file type filters
    ofn.nFilterIndex = 3; // Index of default filter (starting from 1), in this case 3 refers to Gerber files
    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFileTitle = 10;
    ofn.lpstrInitialDir = L"Documents";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

    printf("\nStarting open file dialog box\n");

    // Display the Open dialog box.
    if (GetOpenFileNameW(&ofn)==TRUE)
    {
        wcscpy_s(szFilePath, MAX_PATH, szFile);
        MessageBoxW(hwnd, szFilePath, L"File Opened Successfully", MB_OK | MB_ICONINFORMATION);
        // Open file to get information
        HANDLE hfile = CreateFileW(szFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        // If LoadFile is false, it is assumed only the path to the file is required by the function and the function returns;
        if(LoadFile == false)
        {
            CloseHandle(hfile);
            return 0;
        }

        if(hfile)
        {
            DWORD fileSize = GetFileSize(hfile, NULL);
            if(FileBuffer != NULL)
                delete[] FileBuffer;
            FileBuffer = new CHAR[fileSize+1];    // Extra character to insert a 0 in case the file chosen is not null terminated
            DWORD bytesRead = 0;
            if(ReadFile(hfile, FileBuffer, fileSize, &bytesRead, NULL))
            {
                printf("\nFile read successfully\n");
                if(bytesRead > fileSize)
                {
                    printf("\nERROR: Number of bytes read is more than the maximum allowed!\n");
                    CloseHandle(hfile);
                    return 1;
                }
                FileBuffer[fileSize]= 0;  // Safe null terminator
            }
            else
            {
                MessageBox(NULL, "Failed to read file", "Error", MB_OK | MB_ICONERROR);
                delete[] FileBuffer;
                CloseHandle(hfile);
                return 2;
            }
            CloseHandle(hfile);
        }
        else
        {
            MessageBox(NULL, "Failed to create file handle", "Error", MB_OK | MB_ICONERROR);
            return 3;
        }

        // Invalidate window to force it to completely redraw
        InvalidateRect(hwnd, NULL, FALSE);
    }
    else
    {
        // The function in the if statement returns 0 if the dialog failed because the user canceled. If this is not the case, generate an error message.
        if(CommDlgExtendedError() != 0)
        {
            MessageBox(NULL, "Failed to open file", "Error", MB_OK | MB_ICONERROR);
            return 4;
        }
        return 0;
    }

    return 0;
}

int App_GetTraceInputs(DlgStrct_MaxCopper& result, HWND hwndParent)
{
    if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MAXCPR), hwndParent, (DLGPROC)DlgProc_MaxCopper, (LPARAM)(&result)) == ID_OK)
    {
        if(!result.valid)
        {
            MessageBox(NULL, "Please enter correct numbers in the text fields", "Error", MB_OK | MB_ICONERROR);
            return 0;
        }
    }
    else
    {
        return 0;
    }
    return 1;
}

int App_BitmaptoMMG(bitmap_image* pTraceImage, std::string& mmg, const DlgStrct_MaxCopper& result, std::vector<Command>& pixCmds, bool simplify)
{
    PixelMatrix pm(pTraceImage->width(), pTraceImage->height());
    for(int i = 0; i < pTraceImage->width(); i++)
    {
        for(int j = 0; j < pTraceImage->height(); j++)
        {
            rgb_t color = pTraceImage->get_pixel(i, j);
            if(color.red != 255 || color.green != 255 || color.blue != 255)
            {
                pm.SetPixelState(i, j, ISOLATE);
            }
        }
    }
    pixCmds = TracePixelMatrix(&pm, result.zTop, result.zBottom);
    if(simplify)
        pixCmds = SimplifyCommandsXY(pixCmds);
    mmg = CommandsString(pixCmds, result.pixTomm, true);
    return 1;
}

int App_SaveMMG(const LPWSTR swzPath, const std::string& mmg)
{
    HANDLE hfile_mmg = CreateFileW(swzPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hfile_mmg != INVALID_HANDLE_VALUE)
    {
        DWORD writtenBytes;
        if(WriteFile(hfile_mmg, mmg.c_str(), mmg.size(), &writtenBytes, NULL))
        {
            printf("Successfully written the output file out.mmg with total amount of %d bytes written\n", writtenBytes);
        }
        else
        {
            printf("ERROR ## Failed to write the output file out.mmg with total amount of %d bytes written\n", writtenBytes);
            return 0;
        }
        CloseHandle(hfile_mmg);
    }
    else
    {
        printf("ERROR ## Failed to create the output mmg file\n");
        return 0;
    }
    return 1;
}

int App_SaveImageFromPixCmds(const LPWSTR swzPath, const std::vector<Command>& pixCmds, unsigned int width, unsigned int height)
{
    // Convert wide string to an ascii string
    wstring tempW(swzPath);
    string temp(tempW.begin(), tempW.end());

    bitmap_image img2(width, height);
    img2.set_all_channels(255);
    DetailedCheck(pixCmds, img2).img.save_image(temp);
    return 1;
}

int App_MMGtoCMDs(const std::string& mmg, std::vector<OutCommand>& cmds, std::vector<Command>& pixCmds, const float mmPerStep, const float pixTomm)
{
    //int stepPermm = round(1.0/mmPerStep);

    vector<CompressedCommand> cmpCmds(pixCmds.size());
    for(int i = 0; i < pixCmds.size(); i++)
    {
        cmpCmds[i].x = pixCmds[i].GetX()*pixTomm;
        cmpCmds[i].y = pixCmds[i].GetY()*pixTomm*-1;
        cmpCmds[i].z = pixCmds[i].GetZ()*pixTomm;
    }
    SetMaxSpeed(7);
    cmds = step_mov(cmpCmds, mmPerStep, mmPerStep, mmPerStep);
    return 1;
}

int App_MMGtoCMDs(const std::string& mmg, std::vector<OutCommand>& cmds, std::vector<CompressedCommand>& cmpCmds, const float mmPerStep)
{
    //int stepPermm = round(1.0/mmPerStep);
    SetMaxSpeed(7);
    cmds = step_mov(cmpCmds, mmPerStep, mmPerStep, mmPerStep);
    return 1;
}

int App_SaveCMDs(const LPWSTR swzPath,  const std::vector<OutCommand>& cmds, HWND hwndParent)
{
    // Convert the vector to a buffer
    int l = cmds.size();
    __int8* outBuff = new __int8[l*4];
    for(int i = 0; i < l*4; i += 4)
    {
        outBuff[i] = cmds[i/4].x;
        outBuff[i+1] = cmds[i/4].y;
        outBuff[i+2] = cmds[i/4].z;
        outBuff[i+3] = cmds[i/4].acc;
    }

    HANDLE hfile_cmd = CreateFileW(swzPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hfile_cmd != INVALID_HANDLE_VALUE)
    {
        DWORD writtenBytes;
        if(WriteFile(hfile_cmd, outBuff, l*4, &writtenBytes, NULL))
        {
            printf("Successfully written the output file out.hex with total amount of %d bytes written\n", writtenBytes);
        }
        else
        {
            printf("ERROR ## Failed to write the output file out.hex with total amount of %d bytes written\n", writtenBytes);
        }
        CloseHandle(hfile_cmd);
    }
    else
    {
        printf("ERROR ## Failed to create the output hex file\n");
    }
    delete[] outBuff;
}
