#include "pch.h"
#include "CommunicationService.h"
#include <random>

CommunicationService::CommunicationService(const std::wstring password ): _pipeManager(password){
}
CommunicationService::CommunicationService(): _currentPassword(CreateConnectionKey()), _pipeManager(_currentPassword) {
}

std::wstring CommunicationService::GetCurrentPasswordConnection() {
    return _currentPassword;
}

void CommunicationService::SetConnectionPassword(std::wstring &password ) {
    _currentPassword = password;
}

bool CommunicationService::SendDataToPipe(const StructMessage& data) {
    if (_pipeManager.ConnectToPipe(_currentPassword)) {
        return _pipeManager.SendMessageW(data);
    }
    return false;
}

StructMessage CommunicationService::ReceiveDataFromPipe() {
    if (_pipeManager.ConnectToPipe(_currentPassword)) {
        return _pipeManager.ReceiveMessage();
    }
    else {
        StructMessage message;
        message.Content = "Error";
        return message;
    }
}

std::wstring CommunicationService::CreateConnectionKey() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::wstring caracters = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::wstring password;

    // Gera a senha de 6 caracteres
    for (int i = 0; i < 6; ++i) {
        std::uniform_int_distribution<int> distribuicao(0, caracters.size() - 1);
        password.push_back(caracters[distribuicao(generator)]);
    }
    return password;
} 