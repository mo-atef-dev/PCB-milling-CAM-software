#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include "resource.h"

using namespace std;

/// Child windows IDs
#define ID_CHILD_Layers 50001

/// File global variables
extern WCHAR szGerberPath[];
extern CHAR* szGerberBuffer;
extern string mmgString;
extern HMENU menu;

/// Global functions
int App_Initialize(HINSTANCE hThisInstance);
HWND App_CreateLayersWindow(HWND hwnd);
int App_ResizeLayersWindow(HWND hwndParent);
int App_OpenFile(HWND hwnd);

/// Proc structures
/// These structures represent data communication between the proc functions and the caller
struct DlgStrct_MaxCopper
{
    bool valid;
    float pixTomm;
    int zTop;
    int zBottom;
};

/// Proc functions
BOOL CALLBACK DlgProc_MaxCopper(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc_Layers(HWND, UINT, WPARAM, LPARAM);

#endif // APP_H_INCLUDED
