#include <windows.h>
#include <wininet.h>
#include <string>
#include <iostream>

#pragma comment(lib, "wininet.lib")

auto check_status_from_url() -> std::string
{
    HINTERNET hInternet = InternetOpen("StatusChecker", INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);
    if (!hInternet) return "error";

    HINTERNET hFile = InternetOpenUrl(
        hInternet,
        "https://raw.githubusercontent.com/nyxpdb/Status-Apllication/main/status.txt",
        nullptr, 0, INTERNET_FLAG_RELOAD, 0);

    if (!hFile) {
        InternetCloseHandle(hInternet);
        return "error";
    }

    char buffer[64];
    DWORD bytesRead;
    std::string result;

    if (InternetReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead)) {
        buffer[bytesRead] = '\0';
        result = std::string(buffer);
        result.erase(result.find_last_not_of(" \n\r\t") + 1);
    }

    InternetCloseHandle(hFile);
    InternetCloseHandle(hInternet);

    return result;
}

auto status_check() -> void
{
    std::string status = check_status_from_url();

    if (status == "offline") {
        MessageBoxA(nullptr, "The system is currently offline.\nO sistema atualmente offline.", "System Status", MB_ICONERROR | MB_OK);
        exit(0);
    }
    else if (status == "dev") {
        MessageBoxA(nullptr, "The system is in development mode.\nO sistema em modo de desenvolvimento.", "System Status", MB_ICONWARNING | MB_OK);
        exit(0);
    }
    else if (status != "online") {
        MessageBoxA(nullptr, "Failed to check the system status.\nFalha ao verificar o status do sistema.", "System Status", MB_ICONERROR | MB_OK);
        exit(0);
    }
}

auto main() -> int
{
    status_check();
    std::cout << "Hello, world!" << std::endl;
    return 0;
}
