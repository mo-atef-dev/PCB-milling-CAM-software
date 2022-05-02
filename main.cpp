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
#include "cmds.h"
#include "tracebmp.h"
#include "tracecheck.h"
#include "app.h"

// My header files
#include "resource.h"

// Software constants
#define ZOOMIN_FAC 1.1
#define ZOOMOUT_FAC 0.9

// Headers from other team members
#include "cmncontrols.h"
#include "maxCupper_loading.h"

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
WCHAR szGerberPath[MAX_PATH] = L"";
CHAR* szGerberBuffer = NULL;

WCHAR szDrillPath[MAX_PATH] = L"";
CHAR* szDrillBuffer = NULL;

WCHAR szBorderPath[MAX_PATH] = L"";
CHAR* szBorderBuffer = NULL;

string mmgString;

WCHAR szTracePath[MAX_PATH] = L"";
bitmap_image* pTraceImage;


/* Global variables for drawing window coordinates */
FLOAT origin_x = 0;
FLOAT origin_y = 0;
FLOAT img_scale = 1;

/* Constants for program parameters */
const int layersWidth = 300;
const float mmPerStep = 0.025;

/* Other variables */
HMENU menu;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the main windowclass */

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
    if (!RegisterClassEx(&wincl))
    {
        MessageBox(NULL, "Couldn't create the main window!", "ERROR", MB_OK | MB_ICONERROR);
        return -1;
    }

    // Check that app initialization occurs correctly
    if(App_Initialize(hThisInstance))
    {
        MessageBox(NULL, "Couldn't initialize the app", "ERROR", MB_OK | MB_ICONERROR);
        return -1;
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

    if(szGerberBuffer)
        delete[] szGerberBuffer;

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
    static HWND layerWnd = NULL;
    static HWND drawWnd = NULL;

    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            menu = GetMenu(hwnd);

            // Create the status bar
            status_bar.InitBar(hwnd, 0, GetModuleHandle(NULL), 4);
            status_bar.UpdateText(0, "Status text");

            // Create the side layers window
            layerWnd = App_CreateLayersWindow(hwnd);
            drawWnd = App_CreateDrawingWindow(hwnd);
            if(layerWnd == NULL || drawWnd == NULL)
            {
                exit(-1);
            }

        break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_SIZE:
            {
                /// Resize child windows
                RECT rectWnd;
                GetClientRect(hwnd, &rectWnd);
                // Resize layers window
                MoveWindow(layerWnd, rectWnd.right - layersWidth, 0, layersWidth, rectWnd.bottom - 22, TRUE);
                // Resize drawing window
                MoveWindow(drawWnd, 0, 0, rectWnd.right - layersWidth, rectWnd.bottom - 22, TRUE);
                // Resize status bar
                status_bar.ResizeBar();
            }
            break;
        case WM_LBUTTONDOWN:
        {
            bLButtonHold = TRUE;
            init_mouse_x = GET_X_LPARAM(lParam);
            init_mouse_y = GET_Y_LPARAM(lParam);
            printf("\nLeft mouse clicked\n");
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
                    App_OpenGbrFile(hwnd);
                break;
            case ID_OPENDRILL:
                    App_OpenDrillFile(hwnd);
                break;
            case ID_OPENBORD:
                    App_OpenBorderFile(hwnd);
                break;
            case ID_OPENTRACE:
                    App_OpenTraceImage(hwnd);
                break;
            case ID_MMG:
                {
                    DlgStrct_MaxCopper result;
                    if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MAXCPR), hwnd, (DLGPROC)DlgProc_MaxCopper, (LPARAM)(&result)) == ID_OK)
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
                    vector<Command> traceCmds = TracePixelMatrix(&pm, result.zTop, result.zBottom);
                    mmgString = CommandsString(traceCmds, result.pixTomm, true);

                    /// Create a file to store the resulting mmg commands
                    HANDLE hfile_mmg = CreateFileW(L"./out.mmg", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                    if(hfile_mmg != INVALID_HANDLE_VALUE)
                    {
                        DWORD writtenBytes;
                        if(WriteFile(hfile_mmg, mmgString.c_str(), mmgString.size(), &writtenBytes, NULL))
                        {
                            printf("Successfully written the output file out.mmg with total amount of %d bytes written\n", writtenBytes);
                        }
                        else
                        {
                            printf("ERROR ## Failed to write the output file out.mmg with total amount of %d bytes written\n", writtenBytes);
                        }
                        CloseHandle(hfile_mmg);
                    }
                    else
                    {
                        printf("ERROR ## Failed to create the output mmg file\n");
                    }

                    /// Draw the output image

                    bitmap_image img2(pTraceImage->width(), pTraceImage->height());
                    img2.set_all_channels(255);
                    DetailedCheck(traceCmds, img2).img.save_image("out.bmp");

                    ///

                    printf("Finished mmg file creation\n");

                    /// Create a file to store the resulting compressed commands
                    /// Use the resulting mmg commands to generate the compressed 4 byte commands
                    vector<CompressedCommand> cmpCmds(traceCmds.size());
                    for(int i = 0; i < traceCmds.size(); i++)
                    {
                        cmpCmds[i].x = traceCmds[i].GetX()*result.pixTomm;
                        cmpCmds[i].y = traceCmds[i].GetY()*result.pixTomm*-1;
                        cmpCmds[i].z = traceCmds[i].GetZ()*result.pixTomm;
                    }
                    vector<OutCommand> outCmds = step_mov(cmpCmds, mmPerStep, mmPerStep, mmPerStep);

                    // Convert the vector to a buffer
                    int l = outCmds.size();
                    __int8* outBuff = new __int8[l*4];
                    for(int i = 0; i < l; i += 4)
                    {
                        outBuff[i] = outCmds[i/4].x;
                        outBuff[i+1] = outCmds[i/4].y;
                        outBuff[i+2] = outCmds[i/4].z;
                        outBuff[i+3] = outCmds[i/4].acc;
                    }

                    HANDLE hfile_cmd = CreateFileW(L"./out.hex", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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
                    MessageBox(hwnd, "Successfully created files out.hex, out.mmg, and out.bmp", "Success", MB_OK | MB_ICONINFORMATION);
                }
                break;
            case ID_MAXCPR:

                if(!szDrillBuffer)
                {
                    if(MessageBox(hwnd, "No drill file is opened, do you want to continue?", "Warning", MB_YESNO | MB_ICONEXCLAMATION) != IDYES)
                    {
                        return 0;
                    }
                }

                if(!szBorderBuffer)
                {
                    MessageBox(hwnd, "Please open a Gerber border layer file first", "Error", MB_OK | MB_ICONERROR);
                    return 0;
                }

                if(szGerberBuffer)
                {
                    DlgStrct_MaxCopper result;
                    if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MAXCPR), hwnd, (DLGPROC)DlgProc_MaxCopper, (LPARAM)(&result)) == ID_OK)
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
                    DrawGerberOnBitmab(hwnd, szGerberBuffer, szBorderBuffer, szDrillBuffer, NULL, NULL, NULL, NULL, NULL);
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
                    HANDLE hfile_mmg = CreateFileW(L"./out.mmg", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                    if(hfile_mmg != INVALID_HANDLE_VALUE)
                    {
                        DWORD writtenBytes;
                        if(WriteFile(hfile_mmg, mmgString.c_str(), mmgString.size(), &writtenBytes, NULL))
                        {
                            printf("Successfully written the output file out.mmg with total amount of %d bytes written\n", writtenBytes);
                        }
                        else
                        {
                            printf("ERROR ## Failed to write the output file out.mmg with total amount of %d bytes written\n", writtenBytes);
                        }
                        CloseHandle(hfile_mmg);
                    }
                    else
                    {
                        printf("ERROR ## Failed to create the output mmg file\n");
                    }

                    /// Draw the output image

                    bitmap_image img2(bitmapObject.GetWidth(), bitmapObject.GetHeight());
                    img2.set_all_channels(255);
                    DetailedCheck(traceCmds, img2).img.save_image("out.bmp");

                    ///

                    printf("Finished mmg file creation\n");

                    /// Create a file to store the resulting compressed commands
                    /// Use the resulting mmg commands to generate the compressed 4 byte commands
                    vector<CompressedCommand> cmpCmds(traceCmds.size());
                    for(int i = 0; i < traceCmds.size(); i++)
                    {
                        cmpCmds[i].x = traceCmds[i].GetX()*result.pixTomm;
                        cmpCmds[i].y = traceCmds[i].GetY()*result.pixTomm*-1;
                        cmpCmds[i].z = traceCmds[i].GetZ()*result.pixTomm;
                    }
                    vector<OutCommand> outCmds = step_mov(cmpCmds, mmPerStep, mmPerStep, mmPerStep);

                    // Convert the vector to a buffer
                    int l = outCmds.size();
                    __int8* outBuff = new __int8[l*4];
                    for(int i = 0; i < l; i += 4)
                    {
                        outBuff[i] = outCmds[i/4].x;
                        outBuff[i+1] = outCmds[i/4].y;
                        outBuff[i+2] = outCmds[i/4].z;
                        outBuff[i+3] = outCmds[i/4].acc;
                    }

                    HANDLE hfile_cmd = CreateFileW(L"./out.hex", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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
                    ///
                }
                else
                {
                    MessageBox(hwnd, "Please open a Gerber copper layer file first", "Error", MB_OK | MB_ICONERROR);
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
