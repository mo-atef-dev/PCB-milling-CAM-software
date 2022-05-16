#include "cmncontrols.h"
#warning "Add protection here against calling the init functions twice from the same object"

// Declare printf to be used for debugging
int printf(const char*, ...);

BOOL DOInitCommonControls()
{
    INITCOMMONCONTROLSEX initstruct = {sizeof(INITCOMMONCONTROLSEX), ICC_BAR_CLASSES};
    BOOL result = InitCommonControlsEx(&initstruct);
    return result;
}

/// Button section

CMNButton::CMNButton()
{
    m_hwnd = NULL;
    m_hwndParent = NULL;
    m_hinst = NULL;
    m_idStatus = 0;
}

CMNButton::CMNButton(HWND hwndParent, int idStatus, HINSTANCE hinst, int x, int y, unsigned int width, unsigned int height, DWORD dwStyles, LPSTR initText)
{
    InitButton(hwndParent, idStatus, hinst, x, y, width, height, dwStyles, initText);
}

void CMNButton::InitButton(HWND hwndParent, int idStatus, HINSTANCE hinst, int x, int y, unsigned int width, unsigned int height, DWORD dwStyles, LPSTR initText)
{
    // Assign values to member variables
    m_hwndParent = hwndParent;
    m_idStatus = idStatus;
    m_hinst = hinst;

    // Ensure that the common control DLL is loaded.
    InitCommonControls();

    // Create the progress window.
    m_hwnd = CreateWindowEx(
        0,                       // no extended styles
        WC_BUTTON,         // name of status bar class
        initText,           // no text when first created
        dwStyles,   // creates a visible child window
        x, y, width, height,              // ignores size and position
        hwndParent,              // handle to parent window
        (HMENU) idStatus,       // child window identifier
        hinst,                   // handle to application instance
        NULL);                   // no window creation data

    if(!m_hwnd) printf("Failed to create button control\n");

    if(!ShowWindow(m_hwnd, SW_NORMAL)) printf("Failed to show button control\n");
}

/// Text box section

CMNTextBox::CMNTextBox()
{
    m_hwnd = NULL;
    m_hwndParent = NULL;
    m_hinst = NULL;
    m_idStatus = 0;
}

CMNTextBox::CMNTextBox(HWND hwndParent, int idStatus, HINSTANCE hinst, int x, int y, unsigned int width, unsigned int height, DWORD dwStyles, LPSTR initText)
{
    InitBox(hwndParent, idStatus, hinst, x, y, width, height, dwStyles, initText);
}

void CMNTextBox::InitBox(HWND hwndParent, int idStatus, HINSTANCE hinst, int x, int y, unsigned int width, unsigned int height, DWORD dwStyles, LPSTR initText)
{
    // Assign values to member variables
    m_hwndParent = hwndParent;
    m_idStatus = idStatus;
    m_hinst = hinst;

    // Ensure that the common control DLL is loaded.
    InitCommonControls();

    // Create the progress window.
    m_hwnd = CreateWindowEx(
        0,                       // no extended styles
        WC_EDIT,         // name of status bar class
        initText,           // no text when first created
        dwStyles,   // creates a visible child window
        x, y, width, height,              // ignores size and position
        hwndParent,              // handle to parent window
        (HMENU) idStatus,       // child window identifier
        hinst,                   // handle to application instance
        NULL);                   // no window creation data

    if(!m_hwnd) printf("Failed to create edit control\n");

    printf("Created edit control\n");

    if(!ShowWindow(m_hwnd, SW_NORMAL)) printf("Failed to show edit control\n");
}

BOOL CMNTextBox::GetText(LPSTR szBuffer, DWORD dwSize)
{
    if(m_hwnd == NULL) return false;
    Edit_GetText(m_hwnd, szBuffer, dwSize);
}
BOOL CMNTextBox::SetText(LPSTR szBuffer, DWORD dwSize)
{
    if(m_hwnd == NULL) return false;
    Edit_SetText(m_hwnd, szBuffer);
}

BOOL CMNTextBox::GetText(LPWSTR szBuffer, DWORD dwSize)
{
    if(m_hwnd == NULL) return false;
    GetWindowTextW(m_hwnd, szBuffer, dwSize);
}
BOOL CMNTextBox::SetText(LPWSTR szBuffer, DWORD dwSize)
{
    if(m_hwnd == NULL) return false;
    SetWindowTextW(m_hwnd, szBuffer);
}

/// Label section

CMNLabel::CMNLabel()
{
    m_hwnd = NULL;
    m_hwndParent = NULL;
    m_hinst = NULL;
    m_idStatus = 0;
}

CMNLabel::CMNLabel(HWND hwndParent, int idStatus, HINSTANCE hinst, int x, int y, unsigned int width, unsigned int height, DWORD dwStyles, LPSTR initText)
{
    InitLabel(hwndParent, idStatus, hinst, x, y, width, height, dwStyles, initText);
}

void CMNLabel::InitLabel(HWND hwndParent, int idStatus, HINSTANCE hinst, int x, int y, unsigned int width, unsigned int height, DWORD dwStyles, LPSTR initText)
{
    // Assign values to member variables
    m_hwndParent = hwndParent;
    m_idStatus = idStatus;
    m_hinst = hinst;

    // Ensure that the common control DLL is loaded.
    InitCommonControls();

    // Create the progress window.
    m_hwnd = CreateWindowEx(
        0,                       // no extended styles
        WC_STATIC,         // name of status bar class
        initText,           // no text when first created
        dwStyles,   // creates a visible child window
        x, y, width, height,              // ignores size and position
        hwndParent,              // handle to parent window
        (HMENU) idStatus,       // child window identifier
        hinst,                   // handle to application instance
        NULL);                   // no window creation data

    if(!m_hwnd) printf("Failed to create label control\n");

    if(!ShowWindow(m_hwnd, SW_NORMAL)) printf("Failed to show label control\n");
}

BOOL CMNLabel::GetText(LPSTR szBuffer, DWORD dwSize)
{
    if(m_hwnd == NULL) return false;
    Static_GetText(m_hwnd, szBuffer, dwSize);
}
BOOL CMNLabel::SetText(LPSTR szBuffer, DWORD dwSize)
{
    if(m_hwnd == NULL) return false;
    Static_SetText(m_hwnd, szBuffer);
}
BOOL CMNLabel::GetText(LPWSTR szBuffer, DWORD dwSize)
{
    if(m_hwnd == NULL) return false;
    GetWindowTextW(m_hwnd, szBuffer, dwSize);
}
BOOL CMNLabel::SetText(LPWSTR szBuffer, DWORD dwSize)
{
    if(m_hwnd == NULL) return false;
    SetWindowTextW(m_hwnd, szBuffer);
}

/// Progress window section

CMNProgressWindow::CMNProgressWindow()
{
    m_hwnd = NULL;
    m_hwndParent = NULL;
    m_hinst = NULL;
    m_idStatus = 0;
}

CMNProgressWindow::CMNProgressWindow(HWND hwndParent, int idStatus, HINSTANCE hinst)
{
    InitWindow(hwndParent, idStatus, hinst);
}

void CMNProgressWindow::InitWindow(HWND hwndParent, int idStatus, HINSTANCE hinst)
{
    // Assign values to member variables
    m_hwndParent = hwndParent;
    m_idStatus = idStatus;
    m_hinst = hinst;

    // Ensure that the common control DLL is loaded.
    InitCommonControls();

    // Create the progress window.
    m_hwnd = CreateWindowEx(
        0,                       // no extended styles
        PROGRESS_CLASS,         // name of status bar class
        "Progress Window",           // no text when first created
        PBS_MARQUEE |         // includes a sizing grip
        0,   // creates a visible child window
        0, 0, 0, 0,              // ignores size and position
        hwndParent,              // handle to parent window
        (HMENU) idStatus,       // child window identifier
        hinst,                   // handle to application instance
        NULL);                   // no window creation data

    ShowWindow(m_hwnd, SW_NORMAL);
}

HWND CMNProgressWindow::GetHandler()
{
    return m_hwnd;
}

/// Status bar section

CMNStatusBar::CMNStatusBar()
{
    m_hwnd = NULL;
    m_hwndParent = NULL;
    m_hinst = NULL;
    m_idStatus = 0;
}

CMNStatusBar::CMNStatusBar(HWND hwndParent, int idStatus, HINSTANCE hinst, int cParts)
{
    InitBar(hwndParent, idStatus, hinst, cParts);
}

BOOL CMNStatusBar::UpdateText(int iPart, const char* szText)
{
    LPARAM lParam;
    WPARAM wParam;

    // In case the index is more than the final element
    if(iPart >= m_cParts)
    {
        printf("Warning ## Setting the text of an out-of-bound index (index: %d, limit: %d), index will be changed to the last index\n", iPart, m_cParts);
        iPart = m_cParts-1;
    }

    // Set the part number to be updated (LOBYTE) and the drawing flags (HIBYTE)
    wParam = MAKEWPARAM(iPart, SBT_POPOUT);

    // Set the string to be drawn
    lParam = (LPARAM) szText;

    SendMessage(m_hwnd, SB_SETTEXT, wParam, lParam);
}

void CMNStatusBar::InitBar(HWND hwndParent, int idStatus, HINSTANCE hinst, int cParts)
{
    HWND hwndStatus;
    RECT rcClient;
    HLOCAL hloc;
    PINT paParts;
    int i, nWidth;

    // Assign values to member variables
    m_hwndParent = hwndParent;
    m_idStatus = idStatus;
    m_hinst = hinst;
    m_cParts = cParts;

    // Ensure that the common control DLL is loaded.
    InitCommonControls();

    // Create the status bar.
    hwndStatus = CreateWindowEx(
        0,                       // no extended styles
        STATUSCLASSNAME,         // name of status bar class
        (PCTSTR) NULL,           // no text when first created
        SBARS_SIZEGRIP |         // includes a sizing grip
        WS_CHILD | WS_VISIBLE,   // creates a visible child window
        0, 0, 0, 0,              // ignores size and position
        hwndParent,              // handle to parent window
        (HMENU) idStatus,       // child window identifier
        hinst,                   // handle to application instance
        NULL);                   // no window creation data

    // Get the coordinates of the parent window's client area.
    GetClientRect(hwndParent, &rcClient);

    // Allocate an array for holding the right edge coordinates.
    hloc = LocalAlloc(LHND, sizeof(int) * cParts);
    paParts = (PINT) LocalLock(hloc);

    // Calculate the right edge coordinate for each part, and
    // copy the coordinates to the array.
    nWidth = rcClient.right / cParts;
    int rightEdge = nWidth;
    for (i = 0; i < cParts; i++) {
       paParts[i] = rightEdge;
       rightEdge += nWidth;
    }

    // Tell the status bar to create the window parts.
    SendMessage(hwndStatus, SB_SETPARTS, (WPARAM) cParts, (LPARAM)
               paParts);

    // Free the array, and return.
    LocalUnlock(hloc);
    LocalFree(hloc);
    m_hwnd = hwndStatus;
}

BOOL CMNStatusBar::ResizeBar()
{
    int paParts[m_cParts];
    SendMessage(m_hwnd, WM_SIZE, 0, 0);

    RECT rcClient;
    GetClientRect(m_hwndParent, &rcClient);

    int nWidth = rcClient.right / m_cParts;
    int rightEdge = nWidth;
    for (int i = 0; i < m_cParts; i++) {
       paParts[i] = rightEdge;
       rightEdge += nWidth;
    }

    // Tell the status bar to create the window parts.
    SendMessage(m_hwnd, SB_SETPARTS, (WPARAM) m_cParts, (LPARAM) paParts);
}

HWND CMNStatusBar::GetHandler()
{
    return m_hwnd;
}
