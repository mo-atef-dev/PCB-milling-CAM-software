#ifndef CMNCONTROLS_H_INCLUDED
#define CMNCONTROLS_H_INCLUDED

#include <windows.h>
#include <commctrl.h>

BOOL DOInitCommonControls();

class CMNStatusBar
{
private:
    HWND m_hwnd;
    HWND m_hwndParent;
    int m_idStatus;
    HINSTANCE m_hinst;
    int m_cParts;
public:
    CMNStatusBar();
    CMNStatusBar(HWND hwndParent, int idStatus, HINSTANCE hinst, int cParts);
    void InitBar(HWND hwndParent, int idStatus, HINSTANCE hinst, int cParts);
    BOOL ResizeBar();
    BOOL UpdateText(int iPart, const char* szText);
    HWND GetHandler();
};

class CMNProgressWindow
{
private:
    HWND m_hwnd;
    HWND m_hwndParent;
    int m_idStatus;
    HINSTANCE m_hinst;
public:
    CMNProgressWindow();
    CMNProgressWindow(HWND hwndParent, int idStatus, HINSTANCE hinst);
    void InitWindow(HWND hwndParent, int idStatus, HINSTANCE hinst);
    HWND GetHandler();
};

#endif // CMNCONTROLS_H_INCLUDED
