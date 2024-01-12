#include "pch.h"
#include "NamedPipeManager.h"
#include "NamedPipeConstant.h"

NamedPipeManager::NamedPipeManager(const std::wstring& password) {
    _pipeName = NAMED_PIPE_SERVICE;
    _pipePassword = password;
    hPipe = INVALID_HANDLE_VALUE;
}

NamedPipeManager::~NamedPipeManager() {
    if (hPipe != INVALID_HANDLE_VALUE) {
        CloseHandle(hPipe);
    }
}

bool NamedPipeManager::ConnectToPipe(const std::wstring& password) {
    if (password != _pipePassword) {
        hPipe = CreateFile(_pipeName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        _pipePassword = password;
        return hPipe != INVALID_HANDLE_VALUE;
    }
    return false;
}

bool NamedPipeManager::SendMessageW(const StructMessage& message) {
    std::string serializedMessage = message.Serialize();
    DWORD dwWritten;

    return WriteFile(hPipe, serializedMessage.c_str(), serializedMessage.size(), &dwWritten, NULL) != FALSE;
}

StructMessage NamedPipeManager::ReceiveMessage() {
    char buffer[1024];
    DWORD dwRead;
    ReadFile(hPipe, buffer, sizeof(buffer), &dwRead, NULL);
    std::string serializedMessage(buffer, dwRead);
    return StructMessage::Deserialize(serializedMessage);
}