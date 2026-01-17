#include <windows.h>
#include <string>

HWND hLabelX;
HWND hLabelY;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Entry point
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
        L"Mouse Tracker (Win32)",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg) {

    case WM_CREATE:
        hLabelX = CreateWindow(
            L"STATIC", L"Mouse X = 0",
            WS_VISIBLE | WS_CHILD,
            20, 20, 200, 25,
            hwnd, NULL, NULL, NULL
        );

        hLabelY = CreateWindow(
            L"STATIC", L"Mouse Y = 0",
            WS_VISIBLE | WS_CHILD,
            20, 50, 200, 25,
            hwnd, NULL, NULL, NULL
        );
        break;

    // case WM_MOUSEMOVE: {
    //     int x = LOWORD(lParam);
    //     int y = HIWORD(lParam);

    //     std::wstring textX = L"Mouse X = " + std::to_wstring(x);
    //     std::wstring textY = L"Mouse Y = " + std::to_wstring(y);

    //     // SetWindowText(hLabelX, textX.c_str());
    //     // SetWindowText(hLabelY, textY.c_str());
    //     break;
    //}

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
