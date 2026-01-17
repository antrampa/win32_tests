to compile

g++ main.cpp -o UnitConverter.exe -lgdi32 -luser32

OR
g++ main.cpp -o UnitConverter.exe -DUNICODE -D_UNICODE -lgdi32 -luser32

g++ main.cpp -o MouseTracker.exe -DUNICODE -D_UNICODE -mwindows -lgdi32 -luser32

-mwindows to not open the console

Win32 C++ decumentation:

HWND CreateWindowEx(
    DWORD     dwExStyle,   // extended window style
    LPCWSTR   lpClassName, // window class
    LPCWSTR   lpWindowName,// window title
    DWORD     dwStyle,     // window style
    int       X,           // horizontal position
    int       Y,           // vertical position
    int       nWidth,      // width
    int       nHeight,     // height
    HWND      hWndParent,  // parent window
    HMENU     hMenu,       // menu handle or control ID
    HINSTANCE hInstance,   // instance handle
    LPVOID    lpParam      // creation data
);



Parameter by parameter
dwExStyle – extended window style

In your code: 0 → no extended style.

Other common options:

Flag	Meaning
WS_EX_TOPMOST	Always on top
WS_EX_TOOLWINDOW	Small title bar (used for floating tool windows)
WS_EX_CLIENTEDGE	Adds a 3D sunken edge to the client area
WS_EX_WINDOWEDGE	Adds a 3D border to the window
WS_EX_DLGMODALFRAME	Dialog-style frame without maximize/minimize
WS_EX_TRANSPARENT	Paints transparent background
Combine them with `	`

So if you want a floating tool window, you could write:

CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW, CLASS_NAME, L"My Tool", ...);


dwStyle – window style (big one)

Your code: WS_OVERLAPPEDWINDOW

WS_OVERLAPPEDWINDOW is a macro that expands to multiple flags:

WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX


Meaning:

Sub-flag	Effect
WS_OVERLAPPED	Basic window with title bar
WS_CAPTION	Shows the title bar
WS_SYSMENU	Adds the close menu (icon + close button)
WS_THICKFRAME	Resizable border
WS_MINIMIZEBOX	Minimize button
WS_MAXIMIZEBOX	Maximize button


wc.hCursor = LoadCursor(NULL, IDC_ARROW);
Loads the standard arrow cursor.

You can also load:

Hand cursor

Crosshair

Custom cursor

