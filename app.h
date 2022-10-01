#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <cmath>
#include "tracebmp.h"
#include "tracecheck.h"
#include "resource.h"
#include "bitmap_image.hpp"
#include "cmncontrols.h"

#include "cmds.h"
#include "maxCupper_loading.h"
#include "mmgparse.h"

using namespace std;

/// Child windows IDs
#define ID_CHILD_Layers 50001
#define ID_CHILD_Draw   50002

/// This section is for user defined Windows messages
#define WMU_UPDATE WM_USER+100
#define WMU_GETOUTNAME WM_USER+101
#define WMU_ZOOMIN WM_USER+102
#define WMU_ZOOMOUT WM_USER+103
#define WMU_UP WM_USER+104
#define WMU_LEFT WM_USER+105
#define WMU_RIGHT WM_USER+106
#define WMU_DOWN WM_USER+107

/// Software constants
#define ZOOMIN_FAC 1.1
#define ZOOMOUT_FAC 0.9
#define KEY_PAN_DELTA 5
#define MAX_FILENAME 20

/// Global variables for components
extern CMNStatusBar status_bar;

/// File global variables
extern WCHAR szGerberPath[];
extern CHAR* szGerberBuffer;

extern WCHAR szDrillPath[MAX_PATH];
extern CHAR* szDrillBuffer;

extern WCHAR szBorderPath[MAX_PATH];
extern CHAR* szBorderBuffer;

extern WCHAR szTracePath[MAX_PATH];
extern bitmap_image* pTraceImage;

extern WCHAR szMMGPath[MAX_PATH];
extern WCHAR szCmdsPath[MAX_PATH];

extern string mmgString;
extern HMENU menu;

extern const int layersWidth;
extern const int layersPadding;
extern const int layersItemsSpacing;

extern const float pixTomm;

//////////////////
extern FLOAT img_scale;
extern FLOAT origin_x;
extern FLOAT origin_y;
////

/// Proc structures
/// These structures represent data communication between the proc functions and the caller
struct DlgStrct_MaxCopper
{
    bool valid;
    float pixTomm;
    char maxSpd;
    int zTop;
    int zBottom;
    int zHole;
};

/// Global functions
int App_Initialize(HINSTANCE hThisInstance);

HWND App_CreateLayersWindow(HWND hwnd);
HWND App_CreateDrawingWindow(HWND hwnd);

int App_OpenGbrFile(HWND hwnd);
int App_OpenDrillFile(HWND hwnd);
int App_OpenBorderFile(HWND hwnd);
int App_OpenTraceImage(HWND hwnd);
int App_OpenCopperImage(HWND hwnd); // Not used
int App_OpenMMGFile(HWND hwnd);
int App_OpenCommands(HWND hwnd);
int App_OpenFile(HWND hwnd, WCHAR* szFilePath, CHAR* &FileBuffer, BOOL LoadFile, const LPCWSTR lpstrFilter, DWORD nFilterIndex);

int App_GetTraceInputs(DlgStrct_MaxCopper& result, HWND hwndParent);
int App_GetSaveFileName(LPWSTR name, HWND hwndEditParent, DWORD buffSize);
int App_BitmaptoMMG(bitmap_image* pTraceImage, std::string& mmg, const DlgStrct_MaxCopper& result, std::vector<Command>& traceCmds, bool simplify = true);
int App_SaveMMG(const LPWSTR swzPath, const std::string& mmg);
int App_SaveImageFromPixCmds(const LPWSTR swzPath, const std::vector<Command>& pixCmds, unsigned int width, unsigned int height);
int App_MMGtoCMDs(const std::string& mmg, std::vector<OutCommand>& cmds, std::vector<Command>& pixCmds, const float mmPerStep, const float pixTomm, const float mmPerStepZ);
int App_MMGtoCMDs(const std::string& mmg, std::vector<OutCommand>& cmds, std::vector<CompressedCommand>& cmpCmds, const float mmPerStep, const float mmPerStepZ);
int App_SaveCMDs(const LPWSTR swzPath,  const std::vector<OutCommand>& cmds, HWND hwndParent);

/// Proc functions
BOOL CALLBACK DlgProc_MaxCopper(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc_Layers(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WinProc_Draw(HWND, UINT, WPARAM, LPARAM);

/// Multi-threading functions

#endif // APP_H_INCLUDED
