#include "pch.h"
#include "AppUtil.h"
#include <string>
#include <windows.h>

using namespace Util;

std::wstring AppUtil::GetCurrentPcName() {
    char buffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD length = sizeof(buffer);
    std::wstring ws = L"127.0.0.1";
    bool ok = GetComputerNameExA((COMPUTER_NAME_FORMAT)0, buffer, &length);
    if (ok) {
        ws = std::wstring(buffer, buffer + strlen(buffer));
    }
    return  ws;
}