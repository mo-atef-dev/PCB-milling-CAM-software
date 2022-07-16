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
#include <shlwapi.h>
#include "cmds.h"
#include "tracebmp.h"
#include "tracecheck.h"
#include "app.h"

// My header files
#include "resource.h"

// Headers from other team members
#include "cmncontrols.h"
#include "maxCupper_loading.h"
#include "usb.h"
#include "mmgparse.h"

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

/* Global variables for components */
CMNStatusBar status_bar;

/* Global variable to hold opened file */
WCHAR szGerberPath[MAX_PATH] = L"";
CHAR* szGerberBuffer = NULL;

WCHAR szDrillPath[MAX_PATH] = L"";
CHAR* szDrillBuffer = NULL;

WCHAR szBorderPath[MAX_PATH] = L"";
CHAR* szBorderBuffer = NULL;

WCHAR szMMGPath[MAX_PATH] = L"";
WCHAR szCmdsPath[MAX_PATH] = L"";

string mmgString;

WCHAR szTracePath[MAX_PATH] = L"";
bitmap_image* pTraceImage;

WCHAR szOutPath[MAX_PATH] = L"";

/* Constants for layers window layout */
const int layersWidth = 400;
const int layersPadding = 10;
const int layersItemsSpacing = 20;

/* Constants for layers window layout */
const int drawMargin = 10;

/* Program constants */
const float mmPerStep = 0.02;
const int stepPermm = 50;

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
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
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
    initializeDrawing(hwnd);

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
    static HWND layerWnd = NULL;
    static HWND drawWnd = NULL;

    switch (message)                  /* handle the messages */
    {
        case WM_GETMINMAXINFO:
            {
                LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
                lpMMI->ptMinTrackSize.x = 720;
                lpMMI->ptMinTrackSize.y = 720;
            }
            break;
        case WM_CREATE:
            menu = GetMenu(hwnd);

            // Create the status bar
            status_bar.InitBar(hwnd, 0, GetModuleHandle(NULL), 5);
            status_bar.UpdateText(0, " ");
            status_bar.UpdateText(3, "mm");
            status_bar.UpdateText(4, "Zoom : 100.0%");

            // Create the side layers window
            layerWnd = App_CreateLayersWindow(hwnd);
            drawWnd = App_CreateDrawingWindow(hwnd);
            if(layerWnd == NULL || drawWnd == NULL)
            {
                exit(-1);
            }

        break;
        case WM_DESTROY:
            PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_SIZE:
            {
                /// Resize child windows
                RECT rectWnd;
                GetClientRect(hwnd, &rectWnd);
                // Resize layers window
                MoveWindow(layerWnd, rectWnd.right - layersWidth, 0, layersWidth, rectWnd.bottom - 22, TRUE);
                // Resize drawing window
                MoveWindow(drawWnd, drawMargin, drawMargin, rectWnd.right - layersWidth - drawMargin*2, rectWnd.bottom - 22 - drawMargin*2, TRUE);
                // Resize status bar
                status_bar.ResizeBar();
            }
            break;
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
            case ID_OPENMMG:
                    App_OpenMMGFile(hwnd);
                break;
            case ID_OPENCMDS:
                    App_OpenCommands(hwnd);
                break;
            case ID_VCPR:
                disp = 0;
                DrawBitmapOnWindow(drawWnd,pFactory_, pBitmap_2,wicFactory_);
                break;
            case ID_VMAX:
                disp = 1;
                DrawBitmapOnWindow(drawWnd,pFactory_, pBitmap_,wicFactory_);
                break;
            case ID_VBOTH:
                disp = 2;
                DrawBitmapOnWindow(drawWnd,pFactory_, pBitmap_3,wicFactory_);
                break;
            case ID_DOWNLOAD:
                {
                    if(wcslen(szCmdsPath) >= 1)
                    {
                        printf("Commands file path exists, starting download....\n");

                        // Open the file and store in a buffer
                        HANDLE hfile = CreateFileW(szCmdsPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                        unsigned char* FileBuffer = NULL;
                        unsigned char FileName[] = "out.bin";

                        if(hfile)
                        {
                            DWORD fileSize = GetFileSize(hfile, NULL);
                            if(FileBuffer != NULL)
                                delete[] FileBuffer;
                            FileBuffer = new unsigned char[fileSize];    // Extra character to insert a 0 in case the file chosen is not null terminated
                            DWORD bytesRead = 0;
                            if(ReadFile(hfile, FileBuffer, fileSize, &bytesRead, NULL))
                            {
                                printf("\nCommands file read successfully\n");
                                if(bytesRead > fileSize)
                                {
                                    printf("\nERROR: Number of bytes read is more than the maximum allowed!\n");
                                    CloseHandle(hfile);
                                    break;
                                }
                            }
                            else
                            {
                                MessageBox(hwnd, "Failed to read commands file", "Error", MB_OK | MB_ICONERROR);
                                delete[] FileBuffer;
                                CloseHandle(hfile);
                                break;
                            }
                            CloseHandle(hfile);
                        }
                        else
                        {
                            MessageBox(hwnd, "Failed to create commands file handle", "Error", MB_OK | MB_ICONERROR);
                            break;
                        }

                        // Send the file to the USB function
                        int r = 0;

                        for(int i = 0; i < 10; i++)
                        {
                            r = usb_send(FileBuffer, FileName);
                            if(r)
                            {
                                printf("Successfully downloaded the commands\n");
                                MessageBox(hwnd, "Successfully downloaded the commands", "Success", MB_OK | MB_ICONINFORMATION);
                                break;
                            }
                            else
                            {
                                printf("Failed to download commands at try %d\n", i+1);
                            }
                        }
                        if(r == 0)
                        {
                            printf("Failed to download commands after 10 trys\n");
                            MessageBox(hwnd, "Error downloading commands", "Error", MB_OK | MB_ICONERROR);
                        }
                    }
                    else
                    {
                        MessageBox(hwnd, "Please generate or open a commands file", "Failed", MB_OK | MB_ICONEXCLAMATION);
                    }
                }
                break;
            case ID_CMDS:
                {
                    vector<OutCommand> outCmds;
                    vector<CompressedCommand> cmpCmds;
                    char* tempArr;

                    if(mmgString.size() < 2)
                    {
                        MessageBox(hwnd, "Error in generating commands: MMG commands too short", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }

                    tempArr = new char[mmgString.length()+1];
                    strcpy(tempArr, mmgString.c_str());
                    cmpCmds = MMG_PARSE(tempArr);
                    delete[] tempArr;

                    App_MMGtoCMDs(mmgString, outCmds, cmpCmds, mmPerStep);

                    // Add terminating command
                    outCmds.push_back({0, 0, 0, 0});

                    // Save file to the directory of the open MMG
                    BOOL res;
                    wcscpy(szOutPath, szMMGPath);
                    res = PathRemoveFileSpecW(szOutPath);
                    if(!res)
                    {
                        MessageBox(hwnd, "Error in saving file, file not saved", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }
                    res = PathAppendW(szOutPath, L"out.bin");
                    if(!res)
                    {
                        MessageBox(hwnd, "Error in saving file, file not saved", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }
                    App_SaveCMDs(szOutPath, outCmds, hwnd);

                    MessageBox(hwnd, "Successfully created file out.bin", "Success", MB_OK | MB_ICONINFORMATION);
                }
                break;
            case ID_MMG:
                {
                    DlgStrct_MaxCopper result;

                    if(App_GetTraceInputs(result, hwnd) == 0)
                    {
                        return 0;
                    }

                    vector<Command> traceCmds;

                    App_BitmaptoMMG(pTraceImage, mmgString, result, traceCmds, true);

                    // Set the output path to the directory of the image source
                    BOOL res;
                    wcscpy(szOutPath, szTracePath);
                    res = PathRemoveFileSpecW(szOutPath);
                    if(!res)
                    {
                        MessageBox(hwnd, "Error in saving mmg file, file not saved", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }
                    res = PathAppendW(szOutPath, L"out.mmg");
                    if(!res)
                    {
                        MessageBox(hwnd, "Error in saving mmg file, file not saved", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }

                    /// Create a file to store the resulting mmg commands
                    App_SaveMMG(szOutPath, mmgString);

                    // Replace extension to save a bitmap
                    res = PathRenameExtensionW(szOutPath, L".bmp");
                    if(!res)
                    {
                        MessageBox(hwnd, "Error in saving image, file not saved", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }

                    /// Draw the output image
                    App_SaveImageFromPixCmds(szOutPath, traceCmds, pTraceImage->width(), pTraceImage->height());
                    printf("Finished mmg file creation\n");

                    /// Use the resulting mmg commands to generate the compressed 4 byte commands
                    vector<OutCommand> outCmds;

                    App_MMGtoCMDs(mmgString, outCmds, traceCmds, mmPerStep, result.pixTomm);

                    // Add terminating command
                    outCmds.push_back({0, 0, 0, 0});

                    /// Create a file to store the resulting compressed commands
                    // Replace extension to save the binary file
                    res = PathRenameExtensionW(szOutPath, L".bin");
                    if(!res)
                    {
                        MessageBox(hwnd, "Error in saving file, file not saved", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }

                    App_SaveCMDs(szOutPath, outCmds, hwnd);

                    MessageBox(hwnd, "Successfully created files out.bin, out.mmg, and out.bmp", "Success", MB_OK | MB_ICONINFORMATION);
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

                    if(App_GetTraceInputs(result, hwnd) == 0)
                    {
                        return 0;
                    }

                    MessageBox(hwnd, "Generation of Max-Copper will start. Press OK to continue", "Info", MB_OK | MB_ICONINFORMATION);

                    // This part should be called in a different thread
                    DrawGerberOnBitmab(hwnd, szGerberBuffer, szBorderBuffer, szDrillBuffer, result.pixTomm, true);
                    // end

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
                    traceCmds = SimplifyCommandsXY(traceCmds);
                    mmgString = CommandsString(traceCmds, result.pixTomm, true);

                    // Set the output path to the directory of the copper layer source
                    BOOL res;
                    wcscpy(szOutPath, szGerberPath);
                    res = PathRemoveFileSpecW(szOutPath);
                    if(!res)
                    {
                        MessageBox(hwnd, "Error in saving mmg file, file not saved", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }
                    res = PathAppendW(szOutPath, L"out.mmg");
                    if(!res)
                    {
                        MessageBox(hwnd, "Error in saving mmg file, file not saved", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }

                    /// Create a file to store the resulting mmg commands
                    App_SaveMMG(szOutPath, mmgString);

                    // Replace extension to save a bitmap
                    res = PathRenameExtensionW(szOutPath, L".bmp");
                    if(!res)
                    {
                        MessageBox(hwnd, "Error in saving image, file not saved", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }

                    /// Draw the output image
                    App_SaveImageFromPixCmds(szOutPath, traceCmds, bitmapObject.GetWidth(), bitmapObject.GetHeight());
                    printf("Finished mmg file creation\n");

                    /// Use the resulting mmg commands to generate the compressed 4 byte commands
                    vector<OutCommand> outCmds;

                    App_MMGtoCMDs(mmgString, outCmds, traceCmds, mmPerStep, result.pixTomm);

                    // Add terminating command
                    outCmds.push_back({0, 0, 0, 0});

                    /// Create a file to store the resulting compressed commands
                    // Replace extension to save the binary file
                    res = PathRenameExtensionW(szOutPath, L".bin");
                    if(!res)
                    {
                        MessageBox(hwnd, "Error in saving file, file not saved", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }

                    App_SaveCMDs(szOutPath, outCmds, hwnd);
                }
                else
                {
                    MessageBox(hwnd, "Please open a Gerber copper layer file first", "Error", MB_OK | MB_ICONERROR);
                }
                MessageBox(hwnd, "Successfully created files out.bin, out.mmg, and out.bmp", "Success", MB_OK | MB_ICONINFORMATION);
                break;
            }
            SendMessage(layerWnd, WMU_UPDATE, 0, 0);
        }
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
