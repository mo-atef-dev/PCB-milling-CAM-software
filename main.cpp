#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

//#define V_IMAGE

#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <Commdlg.h>
#include "cmncontrols.h"
#include "maxCupper_loading.h"
#include "tracebmp.h"
#include "tracecheck.h"
#include "app.h"

// Graphics header files
#include <gdiplus.h>



using namespace Gdiplus;

// My header files
#include "resource.h"

// Software constants
#define ZOOMIN_FAC 1.1
#define ZOOMOUT_FAC 0.9

// Headers from other team members
#include "Header.h"

// Debug related header includes
#if defined(DEBUG)
#include <stdio.h>
#else
#define printf(...)
#define wprintf(...)
#endif // defined

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("PCBWindow");

/* Global variable to hold opened file */
WCHAR szImageFile[MAX_PATH] = L"";
Bitmap* image = NULL;
CHAR* szFileBuffer = NULL;
string mmgString;

/* Global variables for drawing window coordinates */
REAL origin_x = 0;
REAL origin_y = 0;
REAL img_scale = 1;

/* Other variables */
HMENU menu;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    #if defined(V_IMAGE)
    // Initialize GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    #endif

    /* The main Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));
    wincl.hIconSm = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));
    wincl.hCursor = LoadCursor (NULL, IDC_CROSS);
    wincl.lpszMenuName = MAKEINTRESOURCE(RSRC_MENU);                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
    {
        MessageBox(NULL, "Couldn't create the main window!", "ERROR", MB_OK | MB_ICONERROR);
        return 0;
    }

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("MaxCopperCAM"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           1280,                 /* The programs width */
           720,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    // Window creation check
    if(!hwnd)
    {
        MessageBox(NULL, "Failed to create a window!", "Error", MB_OK | MB_ICONERROR);
    }

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    ///create factory for drawing
    creat_factory();

    /// Init common controls
    DOInitCommonControls();

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    #if defined(V_IMAGE)
    // Terminate GDI+
    GdiplusShutdown(gdiplusToken);
    if(image)
        delete image;
    #else
    if(szFileBuffer)
        delete[] szFileBuffer;
    #endif

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // For mouse button hold operations
    static BOOL bLButtonHold = FALSE;
    static int init_mouse_x = 0;
    static int init_mouse_y = 0;
    static CMNStatusBar status_bar;

    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            menu = GetMenu(hwnd);
            status_bar.InitBar(hwnd, 0, GetModuleHandle(NULL), 4);
            status_bar.UpdateText(0, "Status text");
        break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_SIZE:
            status_bar.ResizeBar();
            break;
        /*
        case WM_PAINT:
        {
            ///resize the window without effect the shape (resize and draw
            Resize(hwnd, szFileBuffer,  img_scale,  origin_x, origin_y);

            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            // In the examples the PAINTSTRUCT and HDC are declared inside the WM_PAINT message. See if this is disadvantageous in some way.
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

            #if defined(V_IMAGE)
            printf("\nEntered WM_Paint message.\nCurrent Image File: ");
            wprintf(szImageFile);

            PAINTSTRUCT pstr;
            HDC hdc = BeginPaint(hwnd, &pstr);

            // Paint image
            Graphics gr(hdc);
            SolidBrush brush(Color( 255, 255, 255));
            #warning "The fill rectangle dimensions are place holders, the dimensions should be obtained from the client window to assure the whole window is filled"
            gr.FillRectangle(&brush, 0, 0, 1280, 720);
            gr.ScaleTransform(img_scale, img_scale);
            gr.TranslateTransform(origin_x, origin_y, MatrixOrderAppend);
            if(szImageFile[0])
            {
                printf("\nImage file valid.\n");
                //Image background(szImageFile, FALSE);
                //Bitmap background(szImageFile, TRUE);
                //CachedBitmap cachedImage(image, &gr);
                Status imgStatus = gr.DrawImage(image, 0, 0);
                //Status imgStatus = gr.DrawCachedBitmap(&cachedImage, 0, 0);
                if(imgStatus != Ok)
                {
                    printf("Error drawing image! Status: %d", imgStatus);
                }
            }
            EndPaint(hwnd, &pstr);
            #else
            // Write code here for gerber version

            PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			// All painting occurs here, between BeginPaint and EndPaint.
			//FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			if(szFileBuffer)
            {
                ///pass the scale_factor,  offset_x, offset_y to the function
                //draw(hwnd, szFileBuffer, img_scale,  origin_x, origin_y);
            }
			EndPaint(hwnd, &ps);
            #endif
        }
        break;
        */
        case WM_LBUTTONDOWN:
        {
            bLButtonHold = TRUE;
            init_mouse_x = GET_X_LPARAM(lParam);
            init_mouse_y = GET_Y_LPARAM(lParam);
            printf("\nLeft mouse clicked\n");

            //printf(szFileBuffer);
        }
        break;
        case WM_LBUTTONUP:
        {
            bLButtonHold = FALSE;
            printf("\nLeft mouse clicked\n");
        }
        break;
        case WM_MOUSEMOVE:
        {
            if(bLButtonHold)
            {
                origin_x += GET_X_LPARAM(lParam) - init_mouse_x;
                origin_y += GET_Y_LPARAM(lParam) - init_mouse_y;
                init_mouse_x = GET_X_LPARAM(lParam);
                init_mouse_y = GET_Y_LPARAM(lParam);
                InvalidateRect(hwnd, NULL, FALSE);
                printf("## Left Button is held ##");
            }
            char xstr[20];
            char ystr[20];
            sprintf_s(xstr, 20, "X : %d", GET_X_LPARAM(lParam));
            sprintf_s(ystr, 20, "Y : %d", GET_Y_LPARAM(lParam));
            status_bar.UpdateText(1, xstr);
            status_bar.UpdateText(2, ystr);
        }
        break;
        case WM_MOUSEWHEEL:
        {
            // To preserve small wheel increments across different messages
            static int delta = 0;
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
            printf("\nMouse wheel action, img_scale = %f\n", img_scale);
        }
        break;
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
            case ID_EXIT:
                PostMessage(hwnd, WM_CLOSE, 0, 0);
                break;
            case ID_OPEN:
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
                        wcscpy_s(szImageFile, MAX_PATH, szFile);
                        MessageBoxW(hwnd, szImageFile, L"File Opened Successfully", MB_OK | MB_ICONINFORMATION);
                        #if defined (V_IMAGE)
                        printf("\nImage file after opening: ");
                        wprintf(szImageFile);
                        if(image != NULL)
                            delete image;
                        image = new Bitmap(szImageFile, TRUE);
                        #else
                        // Open file to get information
                        HANDLE hfile = CreateFileW(szImageFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                        if(hfile)
                        {
                            DWORD fileSize = GetFileSize(hfile, NULL);
                            if(szFileBuffer != NULL)
                                delete[] szFileBuffer;
                            szFileBuffer = new CHAR[fileSize+1];    // Extra character to insert a 0 in case the file chosen is not null terminated
                            DWORD bytesRead = 0;
                            if(ReadFile(hfile, szFileBuffer, fileSize, &bytesRead, NULL))
                            {
                                printf("\nFile read successfully\n");
                                if(bytesRead > fileSize)
                                {
                                    printf("\nERROR: Number of bytes read is more than the maximum allowed!\n");
                                }
                                szFileBuffer[fileSize]= 0;  // Safe null terminator
                                EnableMenuItem(menu, ID_MAXCPR, MF_ENABLED);
                            }
                            else
                            {
                                MessageBox(NULL, "Failed to read file", "Error", MB_OK | MB_ICONERROR);
                                delete[] szFileBuffer;
                            }
                        }
                        else
                        {
                            MessageBox(NULL, "Failed to create file handle", "Error", MB_OK | MB_ICONERROR);
                        }
                        #endif

                        // Invalidate window to force it to completely redraw
                        InvalidateRect(hwnd, NULL, FALSE);
                    }
                    else
                    {
                        MessageBox(NULL, "Failed to open file", "Error", MB_OK | MB_ICONERROR);
                    }
                }
                break;
                case ID_SAVE:

                break;
                case ID_MAXCPR:

                    if(szFileBuffer)
                    {
                        DlgStrct_MaxCopper result;
                        printf("Result address: %x\n", (&result));
                        if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MAXCPR), hwnd, (DLGPROC)DlgProc_MaxCopper, (LPARAM)(&result)) == IDOK)
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

                        MessageBox(hwnd, "Generation of Max-Copper will start. Press OK to continue", "Info", MB_OK | MB_ICONINFORMATION);
                        DrawGerberOnBitmab(hwnd, szFileBuffer, NULL, NULL, NULL, NULL, NULL);
                        PixelMatrix pm(bitmapObject.GetWidth(), bitmapObject.GetHeight());
                        for(int i = 0; i < bitmapObject.GetWidth(); i++)
                        {
                            for(int j = 0; j < bitmapObject.GetHeight(); j++)
                            {
                                if(bitmapObject.isBlack(i, j))
                                {
                                    pm.SetPixelState(i, j, ISOLATE);
                                }
                            }
                        }
                        vector<Command> traceCmds = TracePixelMatrix(&pm, result.zTop, result.zBottom);
                        mmgString = CommandsString(traceCmds, result.pixTomm, true);

                        /// Create a file to store the resulting mmg commands

                        HANDLE hfile = CreateFileW(L"./out.mmg", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                        if(hfile != INVALID_HANDLE_VALUE)
                        {
                            DWORD writtenBytes;
                            if(WriteFile(hfile, mmgString.c_str(), mmgString.size(), &writtenBytes, NULL))
                            {
                                printf("Successfully written the output file out.mmg with total amount of %d bytes written\n", writtenBytes);
                            }
                            else
                            {
                                printf("ERROR ## Failed to write the output file out.mmg with total amount of %d bytes written\n", writtenBytes);
                            }
                            CloseHandle(hfile);
                        }
                        else
                        {
                            printf("ERROR ## Failed to create the output mmg file\n");
                        }

                        ///

                        /// Draw the output image

                        bitmap_image img2(bitmapObject.GetWidth(), bitmapObject.GetHeight());
                        img2.set_all_channels(255);
                        DetailedCheck(traceCmds, img2).img.save_image("out.bmp");

                        ///
                    }
                    else
                    {
                        MessageBox(hwnd, "Please open a file first", "Error", MB_OK | MB_ICONERROR);
                    }

                break;
            }
        }
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
