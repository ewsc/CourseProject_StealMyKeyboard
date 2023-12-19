#include <iostream>
#include <fstream>
#include <string>

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

int main()
{
    std::string domain;
    std::string ipAddress;

    std::cout << "Menu:\n";
    std::cout << "1. Block domain\n";
    std::cout << "2. Unblock domain\n";
    std::cout << "Enter your choice: ";
    int choice;
    std::cin >> choice;

    switch (choice)
    {
        case 1:
            std::cout << "Enter the domain to block: ";
            std::cin >> domain;
            ipAddress = "0.0.0.0";
            if (AddHostEntry(domain, ipAddress))
            {
                std::cout << "Domain blocked successfully." << std::endl;
            }
            else
            {
                std::cout << "Failed to block domain." << std::endl;
            }
            break;
        case 2:
            std::cout << "Enter the domain to unblock: ";
            std::cin >> domain;
            std::cout << "Enter the IP address to unblock: ";
            std::cin >> ipAddress;
            if (RemoveHostEntry(domain, ipAddress))
            {
                std::cout << "Domain unblocked successfully." << std::endl;
            }
            else
            {
                std::cout << "Failed to unblock domain." << std::endl;
            }
            break;
        default:
            std::cout << "Invalid choice." << std::endl;
            break;
    }

    return 0;
}