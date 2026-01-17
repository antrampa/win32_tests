#define UNICODE
#define _UNICODE
#include <windows.h>
#include <string>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

HHOOK g_mouseHook;
HWND g_hwnd;

HWND hLabelX;
HWND hLabelY;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {

    const wchar_t CLASS_NAME[] = L"MouseTrackerWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Global Mouse Tracker",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 350, 200,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    g_hwnd = hwnd;

    ShowWindow(hwnd, nCmdShow);

    // Install global mouse hook
    g_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(g_mouseHook);

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg) {

    case WM_CREATE:
    {
        CreateWindow(L"STATIC", L"Mouse X:",
            WS_VISIBLE | WS_CHILD,
            30, 40, 80, 25,
            hwnd, NULL, NULL, NULL);

        CreateWindow(L"STATIC", L"Mouse Y:",
            WS_VISIBLE | WS_CHILD,
            30, 80, 80, 25,
            hwnd, NULL, NULL, NULL);

        hLabelX = CreateWindow(L"STATIC", L"0",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            120, 40, 150, 25,
            hwnd, NULL, NULL, NULL);

        hLabelY = CreateWindow(L"STATIC", L"0",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            120, 80, 150, 25,
            hwnd, NULL, NULL, NULL);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {

    if (nCode == HC_ACTION) {
        MSLLHOOKSTRUCT* pMouse = (MSLLHOOKSTRUCT*)lParam;

        std::wstring xText = std::to_wstring(pMouse->pt.x);
        std::wstring yText = std::to_wstring(pMouse->pt.y);

        SetWindowText(hLabelX, xText.c_str());
        SetWindowText(hLabelY, yText.c_str());
    }

    return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);
}
