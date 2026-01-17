//RUN
// g++ main.cpp -o UnitConverter.exe -municode -lgdi32 -luser32
// OR
// g++ main.cpp -o UnitConverter.exe -DUNICODE -D_UNICODE -lgdi32 -luser32


#define UNICODE
#define _UNICODE
#include <windows.h>
#include <string>


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Global handles for controls
HWND hWeightInput, hWeightCombo, hWeightResult;
HWND hLengthInput, hLengthCombo, hLengthResult;
HWND hVolumeInput, hVolumeCombo, hVolumeResult;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {

    const wchar_t CLASS_NAME[] = L"UnitConverterWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Unit Converter Utility",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 520, 420,
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

    case WM_CREATE: {

        // ========== WEIGHT GROUP ==========
        CreateWindow(L"BUTTON", L"Weight Converter",
            WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
            20, 20, 460, 110,
            hwnd, NULL, NULL, NULL);

        CreateWindow(L"STATIC", L"Value:",
            WS_VISIBLE | WS_CHILD,
            40, 50, 50, 20,
            hwnd, NULL, NULL, NULL);

        hWeightInput = CreateWindow(L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            100, 50, 100, 25,
            hwnd, NULL, NULL, NULL);

        hWeightCombo = CreateWindow(L"COMBOBOX", L"",
            WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,
            220, 50, 200, 200,
            hwnd, NULL, NULL, NULL);

        SendMessage(hWeightCombo, CB_ADDSTRING, 0, (LPARAM)L"Kg → Pounds");
        SendMessage(hWeightCombo, CB_ADDSTRING, 0, (LPARAM)L"Pounds → Kg");
        SendMessage(hWeightCombo, CB_SETCURSEL, 0, 0);

        CreateWindow(L"BUTTON", L"Convert",
            WS_VISIBLE | WS_CHILD,
            100, 80, 100, 25,
            hwnd, (HMENU)1, NULL, NULL);

        hWeightResult = CreateWindow(L"STATIC", L"Result:",
            WS_VISIBLE | WS_CHILD,
            220, 80, 200, 25,
            hwnd, NULL, NULL, NULL);


        // ========== LENGTH GROUP ==========
        CreateWindow(L"BUTTON", L"Length Converter",
            WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
            20, 150, 460, 110,
            hwnd, NULL, NULL, NULL);

        CreateWindow(L"STATIC", L"Value:",
            WS_VISIBLE | WS_CHILD,
            40, 180, 50, 20,
            hwnd, NULL, NULL, NULL);

        hLengthInput = CreateWindow(L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            100, 180, 100, 25,
            hwnd, NULL, NULL, NULL);

        hLengthCombo = CreateWindow(L"COMBOBOX", L"",
            WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,
            220, 180, 200, 200,
            hwnd, NULL, NULL, NULL);

        SendMessage(hLengthCombo, CB_ADDSTRING, 0, (LPARAM)L"Cm → Inches");
        SendMessage(hLengthCombo, CB_ADDSTRING, 0, (LPARAM)L"Meters → Feet");
        SendMessage(hLengthCombo, CB_SETCURSEL, 0, 0);

        CreateWindow(L"BUTTON", L"Convert",
            WS_VISIBLE | WS_CHILD,
            100, 210, 100, 25,
            hwnd, (HMENU)2, NULL, NULL);

        hLengthResult = CreateWindow(L"STATIC", L"Result:",
            WS_VISIBLE | WS_CHILD,
            220, 210, 200, 25,
            hwnd, NULL, NULL, NULL);


        // ========== VOLUME GROUP ==========
        CreateWindow(L"BUTTON", L"Volume Converter",
            WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
            20, 280, 460, 110,
            hwnd, NULL, NULL, NULL);

        CreateWindow(L"STATIC", L"Value:",
            WS_VISIBLE | WS_CHILD,
            40, 310, 50, 20,
            hwnd, NULL, NULL, NULL);

        hVolumeInput = CreateWindow(L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            100, 310, 100, 25,
            hwnd, NULL, NULL, NULL);

        hVolumeCombo = CreateWindow(L"COMBOBOX", L"",
            WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,
            220, 310, 200, 200,
            hwnd, NULL, NULL, NULL);

        SendMessage(hVolumeCombo, CB_ADDSTRING, 0, (LPARAM)L"Litres → Gallons");
        SendMessage(hVolumeCombo, CB_ADDSTRING, 0, (LPARAM)L"Gallons → Litres");
        SendMessage(hVolumeCombo, CB_SETCURSEL, 0, 0);

        CreateWindow(L"BUTTON", L"Convert",
            WS_VISIBLE | WS_CHILD,
            100, 340, 100, 25,
            hwnd, (HMENU)3, NULL, NULL);

        hVolumeResult = CreateWindow(L"STATIC", L"Result:",
            WS_VISIBLE | WS_CHILD,
            220, 340, 200, 25,
            hwnd, NULL, NULL, NULL);

        break;
    }
        case WM_COMMAND: {

        wchar_t buffer[100];
        double value, result;
        int index;

        // Weight
        if (LOWORD(wParam) == 1) {
            GetWindowTextW(hWeightInput, buffer, 100);
            value = _wtof(buffer);
            index = SendMessage(hWeightCombo, CB_GETCURSEL, 0, 0);

            if (index == 0) result = value * 2.20462;
            if (index == 1) result = value / 2.20462;

            SetWindowTextW(hWeightResult, (L"Result: " + std::to_wstring(result)).c_str());
        }

        // Length
        if (LOWORD(wParam) == 2) {
            GetWindowTextW(hLengthInput, buffer, 100);
            value = _wtof(buffer);
            index = SendMessage(hLengthCombo, CB_GETCURSEL, 0, 0);

            if (index == 0) result = value * 0.393701;
            if (index == 1) result = value * 3.28084;

            SetWindowTextW(hLengthResult, (L"Result: " + std::to_wstring(result)).c_str());
        }

        // Volume
        if (LOWORD(wParam) == 3) {
            GetWindowTextW(hVolumeInput, buffer, 100);
            value = _wtof(buffer);
            index = SendMessage(hVolumeCombo, CB_GETCURSEL, 0, 0);

            if (index == 0) result = value * 0.264172;
            if (index == 1) result = value / 0.264172;

            SetWindowTextW(hVolumeResult, (L"Result: " + std::to_wstring(result)).c_str());
        }

        break;
    }
        case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
