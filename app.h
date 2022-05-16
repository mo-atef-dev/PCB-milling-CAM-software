#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include "resource.h"
#include "bitmap_image.hpp"
#include "cmncontrols.h"

#include "cmds.h"

using namespace std;

/// Child windows IDs
#define ID_CHILD_Layers 50001
#define ID_CHILD_Draw   50002

/// This section is for user defined Windows messages
#define WMU_UPDATE WM_USER+100

/// File global variables
extern WCHAR szGerberPath[];
extern CHAR* szGerberBuffer;

extern WCHAR szDrillPath[MAX_PATH];
extern CHAR* szDrillBuffer;

extern WCHAR szBorderPath[MAX_PATH];
extern CHAR* szBorderBuffer;

extern WCHAR szTracePath[];
extern bitmap_image* pTraceImage;

extern string mmgString;
extern HMENU menu;

extern const int layersWidth;
extern const int layersPadding;
extern const int layersItemsSpacing;

/// Proc structures
/// These structures represent data communication between the proc functions and the caller
struct DlgStrct_MaxCopper
{
    bool valid;
    float pixTomm;
    int zTop;
    int zBottom;
};

/// Global functions
int App_Initialize(HINSTANCE hThisInstance);

HWND App_CreateLayersWindow(HWND hwnd);
HWND App_CreateDrawingWindow(HWND hwnd);

int App_OpenGbrFile(HWND hwnd);
int App_OpenDrillFile(HWND hwnd);
int App_OpenBorderFile(HWND hwnd);
int App_OpenTraceImage(HWND hwnd);
int App_OpenCopperImage(HWND hwnd);
int App_OpenFile(HWND hwnd, WCHAR* szFilePath, CHAR* &FileBuffer, BOOL LoadFile, const LPCWSTR lpstrFilter, DWORD nFilterIndex);

int App_GetTraceInputs(DlgStrct_MaxCopper& result);
int App_PMtoMMG(PixelMatrix* pm, string& mmg);
int App_SaveMMG(LPWSTR swzPath, const string& mmg);
int App_SaveImageFromMMG(LPWSTR swzPath, const string& mmg);
int App_MMGtoCMDs(const string& mmg, std::vector<OutCommand>& cmds, const float mmPerStep);
int App_SaveCMDs(LPWSTR swzPath,  const std::vector<OutCommand>& cmds);

/// Proc functions
BOOL CALLBACK DlgProc_MaxCopper(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc_Layers(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WinProc_Draw(HWND, UINT, WPARAM, LPARAM);

#endif // APP_H_INCLUDED
