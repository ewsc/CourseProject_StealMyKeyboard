#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

HWND hwndBlockButton;
HWND hwndUnblockButton;
HWND hwndDomainInput;
HWND hwndIpAddressInput;

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
            std::cout << "Key pressed: " << vkCode << std::endl;
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
                         hwnd, NULL, NULL, NULL);

            hwndDomainInput = CreateWindow("EDIT", "",
                                           WS_VISIBLE | WS_CHILD | WS_BORDER,
                                           20, 50, 200, 30,
                                           hwnd, NULL, NULL, NULL);

            hwndIpAddressInput = CreateWindow("EDIT", "",
                                              WS_VISIBLE | WS_CHILD | WS_BORDER,
                                              20, 90, 200, 30,
                                              hwnd, NULL, NULL, NULL);

            hwndBlockButton = CreateWindow("BUTTON", "Block",
                                           WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                           20, 130, 150, 30,
                                           hwnd, NULL, NULL, NULL);

            hwndUnblockButton = CreateWindow("BUTTON", "Unblock",
                                             WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                             200, 130, 150, 30,
                                             hwnd, NULL, NULL, NULL);

            HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE));

            // Install the keyboard hook
            HHOOK hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, hInstance, 0);
            if (hKeyboardHook == NULL)
            {
                MessageBox(hwnd, "Failed to install the keyboard hook.", "Error", MB_OK | MB_ICONERROR);
            }
            else
            {
                // Store the hook handle in the window user data
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(hKeyboardHook));
            }
        }
            break;

        case WM_COMMAND:
        {
            if (lParam == (LPARAM)hwndBlockButton)
            {
                char domain[256];
                char ipAddress[256];

                GetWindowTextA(hwndDomainInput, domain, sizeof(domain));
                GetWindowTextA(hwndIpAddressInput, ipAddress, sizeof(ipAddress));

                if (AddHostEntry(domain, ipAddress))
                {
                    MessageBox(hwnd, "Domain blocked successfully.", "Success", MB_OK | MB_ICONINFORMATION);
                }
                else
                {
                    MessageBox(hwnd, "Failed to block domain.", "Error", MB_OK | MB_ICONERROR);
                }
            }
            else if (lParam == (LPARAM)hwndUnblockButton)
            {
                char domain[256];
                char ipAddress[256];

                GetWindowTextA(hwndDomainInput, domain, sizeof(domain));
                GetWindowTextA(hwndIpAddressInput, ipAddress, sizeof(ipAddress));

                if (RemoveHostEntry(domain, ipAddress))
                {
                    MessageBox(hwnd, "Domain unblocked successfully.", "Success", MB_OK | MB_ICONINFORMATION);
                }
                else
                {
                    MessageBox(hwnd, "Failed to unblock domain.", "Error", MB_OK | MB_ICONERROR);
                }
            }
        }
            break;

        case WM_DESTROY:
        {
            // Uninstall the keyboard hook
            HHOOK hKeyboardHook = reinterpret_cast<HHOOK>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            UnhookWindowsHookEx(hKeyboardHook);
            PostQuitMessage(0);
        }
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Register the window class
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "BlockDomainWindowClass";

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, "Window registration failed.", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Create the window
    HWND hwnd = CreateWindow(wc.lpszClassName, "Block Domain",
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                             CW_USEDEFAULT, CW_USEDEFAULT, 400, 250,
                             NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window creation failed.", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}