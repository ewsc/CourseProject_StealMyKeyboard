#include <iostream>
#include <chrono>
#include <ctime>
#include <Windows.h>
#include <wininet.h>

// Function to check if the current time is within the specified range
bool isTimeWithinRange(const std::tm& currentTime, int startHour, int endHour)
{
    int currentHour = currentTime.tm_hour;
    return (currentHour >= startHour && currentHour < endHour);
}

// Function to enable or disable internet access
void setInternetAccess(bool enable)
{
    DWORD dwFlags;
    if (InternetGetConnectedState(&dwFlags, 0))
    {
        HINTERNET hInternet = InternetOpen(reinterpret_cast<LPCSTR>(L"Internet Control"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (hInternet)
        {
            INTERNET_PER_CONN_OPTION_LIST list;
            INTERNET_PER_CONN_OPTION options[1];
            options[0].dwOption = INTERNET_PER_CONN_FLAGS;
            options[0].Value.dwValue = enable ? 0 : PROXY_TYPE_DIRECT;
            list.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
            list.pszConnection = NULL;
            list.dwOptionCount = 1;
            list.dwOptionError = 0;
            list.pOptions = options;

            InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &list, sizeof(INTERNET_PER_CONN_OPTION_LIST));
            InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
            InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL, 0);

            InternetCloseHandle(hInternet);
        }
    }
}

// Button click event handler
LRESULT CALLBACK ButtonClickHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_COMMAND && LOWORD(wParam) == 1001)
    {
        // Get the current local time
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm currentTime;
        localtime_s(&currentTime, &now);

        // Check if the current time is within the specified range (8 am to 8 pm)
        if (isTimeWithinRange(currentTime, 8, 20))
        {
            // Internet access is allowed
            setInternetAccess(true);
            std::cout << "Internet access is allowed.\n";
        }
        else
        {
            // Internet access is blocked
            setInternetAccess(false);
            std::cout << "Internet access is blocked.\n";
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"KeyLogger";

    // Register the window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = ButtonClickHandler;
    wc.hInstance = hInstance;
    wc.lpszClassName = reinterpret_cast<LPCSTR>(CLASS_NAME);

    RegisterClass(&wc);

    // Create the window
    HWND hWnd = CreateWindowEx(
            0,                              // Optional window styles
            reinterpret_cast<LPCSTR>(CLASS_NAME),                     // Window class name
            reinterpret_cast<LPCSTR>(L"Internet Control"),            // Window title
            WS_OVERLAPPEDWINDOW,            // Window style

            // Size and position
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

            NULL,       // Parent window
            NULL,       // Menu
            hInstance,  // Instance handle
            NULL        // Additional application data
    );

    if (hWnd == NULL)
    {
        return 0;
    }

    // Create a button
    HWND hButton = CreateWindow(
            reinterpret_cast<LPCSTR>(L"BUTTON"),                      // Button class
            reinterpret_cast<LPCSTR>(L"Activate Internet Control"),   // Button text
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Button styles
            10, 10, 200, 30,                // Button position and size
            hWnd,                           // Parent window handle
            (HMENU)1001,                    // Button ID
            hInstance,                      // Instance handle
            NULL                            // Additional application data
    );

    if (hButton == NULL)
    {
        return 0;
    }

    // Show the window
    ShowWindow(hWnd, nCmdShow);

    // Run the message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}