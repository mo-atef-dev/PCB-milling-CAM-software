#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "resource.h"

/// Proc structures
/// These structures are global and represent data communication between the proc functions and the caller
struct DlgStrct_MaxCopper
{
    bool valid;
    float pixTomm;
    int zTop;
    int zBottom;
};

/// Proc functions
BOOL CALLBACK DlgProc_MaxCopper(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

#endif // APP_H_INCLUDED
