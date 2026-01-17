// #define UNICODE
// #define _UNICODE
// g++ main.cpp -o MouseTracker.exe -DUNICODE -D_UNICODE -mwindows -lgdi32 -luser32
#include <windows.h>
#include <string>

#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAY_EXIT 1001
#define ID_TRAY_SHOW 1002

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
void ToggleAlwaysOnTop(HWND hwnd);

HHOOK g_hMouseHook;
HWND g_hwnd;
HHOOK g_hKeyboardHook;

HWND hLabelX;
HWND hLabelY;
HWND hLabelKey;

NOTIFYICONDATA nid;

bool isTopMost = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {

    const wchar_t CLASS_NAME[] = L"MouseTrackerWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        CLASS_NAME,
        L"Global Mouse Tracker",
        WS_OVERLAPPEDWINDOW | WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT, 450, 250,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    g_hwnd = hwnd;

    ShowWindow(hwnd, nCmdShow);

    // Install global mouse hook
    g_hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);

    // Install global keyboard hook
    g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(g_hMouseHook);

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg) {

        case WM_CREATE:
        {
            if (!RegisterHotKey(hwnd, 1, MOD_CONTROL | MOD_ALT, 'M')) { // Ctrl + Alt + M to toggle visibility
                MessageBox(hwnd, L"Failed to register Ctrl+Alt+M hotkey!", L"Error", MB_ICONERROR);
            }
            if (!RegisterHotKey(hwnd, 2, MOD_CONTROL | MOD_ALT, 'X')) { // Ctrl + Alt + X to toggle always on top
                MessageBox(hwnd, L"Failed to register Ctrl+Alt+X hotkey!", L"Error", MB_ICONERROR);
            } 
             

            CreateWindow(L"STATIC", L"Mouse X:",
                WS_VISIBLE | WS_CHILD,
                30, 40, 180, 25,
                hwnd, NULL, NULL, NULL);

            CreateWindow(L"STATIC", L"Mouse Y:",
                WS_VISIBLE | WS_CHILD,
                30, 80, 180, 25,
                hwnd, NULL, NULL, NULL);
            
            CreateWindow(L"STATIC", L"Keyboard Key Code:", 
                WS_VISIBLE | WS_CHILD,
                30, 120, 180, 25,
                hwnd, NULL, NULL, NULL);

            hLabelX = CreateWindow(L"STATIC", L"0",
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                200, 40, 200, 25,
                hwnd, NULL, NULL, NULL);

            hLabelY = CreateWindow(L"STATIC", L"0",
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                200, 80, 200, 25,
                hwnd, NULL, NULL, NULL);

            hLabelKey = CreateWindow(L"STATIC", L"",
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                200, 120, 200, 25,
                hwnd, NULL, NULL, NULL);        
                
            //TrayIcon
            nid.cbSize = sizeof(NOTIFYICONDATA);
            nid.hWnd = hwnd;
            nid.uID = 1;
            nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
            nid.uCallbackMessage = WM_TRAYICON;
            nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            wcscpy_s(nid.szTip, L"Mouse Tracker Utility");

            Shell_NotifyIcon(NIM_ADD, &nid);
        }
        break;
        case WM_TRAYICON:
            if(lParam == WM_LBUTTONDBLCLK) {
                ShowWindow(hwnd, SW_SHOW);
                SetForegroundWindow(hwnd);
            } 
            else if(lParam == WM_RBUTTONUP) {
                POINT pt;
                GetCursorPos(&pt); // cursor position

                HMENU hMenu = CreatePopupMenu();
                AppendMenu(hMenu, MF_STRING, ID_TRAY_SHOW, L"Show");
                AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT, L"Exit");

                SetForegroundWindow(hwnd); // needed to make menu show
                TrackPopupMenu(
                    hMenu,
                    TPM_RIGHTBUTTON,
                    pt.x, pt.y,
                    0,
                    hwnd,
                    NULL
                );

                DestroyMenu(hMenu);
            }

            break;
        case WM_HOTKEY:
            if(wParam == 1) {
                if(IsWindowVisible(hwnd)) {
                    ShowWindow(hwnd, SW_HIDE);
                } else {
                    ShowWindow(hwnd, SW_SHOW);
                    SetForegroundWindow(hwnd);    
                }
            }

            if(wParam == 2) {
                ToggleAlwaysOnTop(hwnd);
            }

            break;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_TRAY_SHOW:
                    ShowWindow(hwnd, SW_SHOW);
                    SetForegroundWindow(hwnd);
                    break;
                case ID_TRAY_EXIT:
                    DestroyWindow(hwnd); // this will trigger WM_DESTROY
                    break;
            }
            break;    
        case WM_CLOSE:
            ShowWindow(hwnd, SW_HIDE); // hides window instead of closing
            return 0;
        case WM_DESTROY:
            UnregisterHotKey(hwnd, 1);
            UnregisterHotKey(hwnd, 2);

            Shell_NotifyIcon(NIM_DELETE, &nid);
            UnhookWindowsHookEx(g_hMouseHook);
            UnhookWindowsHookEx(g_hKeyboardHook);

            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {

    if (nCode == HC_ACTION && wParam == WM_MOUSEMOVE) {
        MSLLHOOKSTRUCT* pMouse = (MSLLHOOKSTRUCT*)lParam;

        std::wstring xText = std::to_wstring(pMouse->pt.x);
        std::wstring yText = std::to_wstring(pMouse->pt.y);

        SetWindowText(hLabelX, xText.c_str());
        SetWindowText(hLabelY, yText.c_str());
    }

    return CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;

        if (wParam == WM_KEYDOWN) {
            printf("Key pressed: %d\n", p->vkCode);
            std::wstring keyText = L"" + std::to_wstring(p->vkCode);
            SetWindowText(hLabelKey, keyText.c_str());
        }
    }
    return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
}

void ToggleAlwaysOnTop(HWND hwnd) {
    isTopMost = !isTopMost;

    SetWindowPos(
        hwnd,
        isTopMost ? HWND_TOPMOST : HWND_NOTOPMOST,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE
    );
}