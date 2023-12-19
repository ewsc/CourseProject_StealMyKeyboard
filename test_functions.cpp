#include <iostream>
#include <windows.h>
#include <Iphlpapi.h>
#include <vector>

#pragma comment(lib, "iphlpapi.lib")

int main() {
    DWORD bufferSize = 0;
    DWORD status = GetExtendedTcpTable(nullptr, &bufferSize, false, AF_INET, TCP_TABLE_OWNER_MODULE_ALL, 0);
    if (status != ERROR_INSUFFICIENT_BUFFER) {
        std::cerr << "Failed to retrieve TCP table size: " << status << std::endl;
        return 1;
    }

    std::vector<byte> buffer(bufferSize);
    PMIB_TCPTABLE_OWNER_MODULE tcpTable = reinterpret_cast<PMIB_TCPTABLE_OWNER_MODULE>(buffer.data());
    status = GetExtendedTcpTable(tcpTable, &bufferSize, false, AF_INET, TCP_TABLE_OWNER_MODULE_ALL, 0);
    if (status != NO_ERROR) {
        std::cerr << "Failed to retrieve TCP table: " << status << std::endl;
        return 1;
    }

    for (DWORD i = 0; i < tcpTable->dwNumEntries; ++i) {
        PMIB_TCPROW_OWNER_MODULE row = &tcpTable->table[i];
        std::cout << "Accessed Domain: " << row->dwRemoteAddr << std::endl;
    }

    return 0;
}