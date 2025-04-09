#include "status.hpp"
#include <windows.h>
#include <wininet.h>
#include <iostream>

#pragma comment(lib, "wininet.lib")

const char* MSG_NO_INTERNET = "No internet connection.\nSem conexão com a internet.";
const char* MSG_OFFLINE = "The system is currently offline.\nO sistema está atualmente offline.";
const char* MSG_DEV = "The system is in development mode.\nO sistema está em modo de desenvolvimento.";
const char* MSG_ERROR = "Failed to check the system status.\nFalha ao verificar o status do sistema.";

bool has_internet_connection()
{
    return InternetCheckConnection("http://www.google.com", FLAG_ICC_FORCE_CONNECTION, 0);
}

std::string check_status_from_url()
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

bool status_check()
{
    if (!has_internet_connection()) {
        MessageBoxA(nullptr, MSG_NO_INTERNET, "Network Error", MB_ICONERROR | MB_OK);
        return false;
    }

    std::string status = check_status_from_url();

    if (status == "offline") {
        MessageBoxA(nullptr, MSG_OFFLINE, "System Status", MB_ICONERROR | MB_OK);
        return false;
    }
    else if (status == "dev") {
        MessageBoxA(nullptr, MSG_DEV, "System Status", MB_ICONWARNING | MB_OK);
        return false;
    }
    else if (status != "online") {
        MessageBoxA(nullptr, MSG_ERROR, "System Status", MB_ICONERROR | MB_OK);
        return false;
    }

    std::cout << "[INFO] Status online. Proceeding...\n";
    return true;
}