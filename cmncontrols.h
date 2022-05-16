#ifndef CMNCONTROLS_H_INCLUDED
#define CMNCONTROLS_H_INCLUDED

#include <windows.h>
#include <windowsx.h>
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

// Some definitions for common text book control styles
#define CMNTXT_BASE         WS_CHILD | WS_VISIBLE | WS_BORDER
#define CMNTXT_READONLY     WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY

class CMNTextBox
{
private:
    HWND m_hwnd;
    HWND m_hwndParent;
    int m_idStatus;
    HINSTANCE m_hinst;
public:
    CMNTextBox();
    CMNTextBox(HWND hwndParent, int idStatus, HINSTANCE hinst, int x, int y, unsigned int width, unsigned int height, DWORD dwStyles = WS_CHILD | WS_VISIBLE | WS_BORDER, LPSTR initText = "");
    void InitBox(HWND hwndParent, int idStatus, HINSTANCE hinst, int x, int y, unsigned int width, unsigned int height, DWORD dwStyles = WS_CHILD | WS_VISIBLE | WS_BORDER, LPSTR initText = "");
    HWND GetHandler();
    BOOL GetText(LPSTR szBuffer, DWORD dwSize);
    BOOL SetText(LPSTR szBuffer, DWORD dwSize);
    BOOL GetText(LPWSTR szBuffer, DWORD dwSize);
    BOOL SetText(LPWSTR szBuffer, DWORD dwSize);
};

class CMNLabel
{
private:
    HWND m_hwnd;
    HWND m_hwndParent;
    int m_idStatus;
    HINSTANCE m_hinst;
public:
    CMNLabel();
    CMNLabel(HWND hwndParent, int idStatus, HINSTANCE hinst, int x, int y, unsigned int width, unsigned int height, DWORD dwStyles = WS_CHILD | WS_VISIBLE, LPSTR initText = "");
    void InitLabel(HWND hwndParent, int idStatus, HINSTANCE hinst, int x, int y, unsigned int width, unsigned int height, DWORD dwStyles = WS_CHILD | WS_VISIBLE, LPSTR initText = "");
    HWND GetHandler();
    BOOL GetText(LPSTR szBuffer, DWORD dwSize);
    BOOL SetText(LPSTR szBuffer, DWORD dwSize);
    BOOL GetText(LPWSTR szBuffer, DWORD dwSize);
    BOOL SetText(LPWSTR szBuffer, DWORD dwSize);
};

class CMNButton
{
private:
    HWND m_hwnd;
    HWND m_hwndParent;
    int m_idStatus;
    HINSTANCE m_hinst;
public:
    CMNButton();
    CMNButton(HWND hwndParent, int idStatus, HINSTANCE hinst, int x, int y, unsigned int width, unsigned int height, DWORD dwStyles = WS_CHILD | WS_VISIBLE, LPSTR initText = "");
    void InitButton(HWND hwndParent, int idStatus, HINSTANCE hinst, int x, int y, unsigned int width, unsigned int height, DWORD dwStyles = WS_CHILD | WS_VISIBLE, LPSTR initText = "");
    HWND GetHandler();
    BOOL GetText(LPSTR szBuffer, DWORD dwSize);
    BOOL SetText(LPSTR szBuffer, DWORD dwSize);
    BOOL GetText(LPWSTR szBuffer, DWORD dwSize);
    BOOL SetText(LPWSTR szBuffer, DWORD dwSize);
};

#endif // CMNCONTROLS_H_INCLUDED
