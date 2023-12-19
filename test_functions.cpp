#include <iostream>
#include <Windows.h>
#include "include/windivert.h"

// Function to block internet access
void BlockInternetAccess()
{
    // Create a handle to the WinDivert driver
    HANDLE handle = WinDivertOpen("outbound && ip", WINDIVERT_LAYER_NETWORK, 0, 0);
    if (handle == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Failed to open WinDivert driver. Make sure you have administrative privileges." << std::endl;
        return;
    }

    // Modify the network packets to block internet access
    char packet[65536];
    UINT packetLength;

    while (true)
    {
        if (!WinDivertRecv(handle, packet, sizeof(packet), &packetLength, NULL))
        {
            std::cerr << "Error while receiving packet: " << GetLastError() << std::endl;
            break;
        }

        // Drop the packet to block internet access
        if (!WinDivertSend(handle, packet, packetLength, NULL, NULL))
        {
            std::cerr << "Error while sending packet: " << GetLastError() << std::endl;
            break;
        }
    }

    // Close the handle to the WinDivert driver
    WinDivertClose(handle);
}

// Entry point of the program
int main()
{
    // Create a button-click event mechanism
    std::cout << "Press any key to block internet access...";
    std::cin.ignore();

    // Block internet access when the button is clicked
    BlockInternetAccess();

    return 0;
}