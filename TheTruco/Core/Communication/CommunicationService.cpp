#include "pch.h"
#include <random>
#include <Communication/CommunicationService.h>

using namespace Communication;
using namespace Helpers;

CommunicationService::CommunicationService() : _currentConnectionKey(CreateConnectionKey()), _isCurrentServer(true) {
    DeserializeCurrentMachineName(_currentConnectionKey);
    _pipeManager = std::make_unique<NamedPipeManager>(_currentPassword, _currentMachineName);
}

std::wstring CommunicationService::GetConnectionKey() const {
    return _currentConnectionKey;
}

void CommunicationService::SetConnectionPassword(const std::wstring_view& password) {
    _currentPassword = password;
}

bool CommunicationService::SendMessageAsHost(const StructMessage& data) {
    if (_pipeManager->IsPipeConnected()) {
        return _pipeManager->SendMessageW(data);
    }
    else {
        _pipeManager->ConnectToPipe(_currentPassword);
        SendMessageAsHost(data);
        if (_isCurrentServer) {
            _pipeManager->ConnectToPipe(_currentPassword);
            SendMessageAsHost(data);
        }
        else {
            _pipeManager->CreateClientConnection(_currentPassword);
            SendMessageAsHost(data);
        }
    }
    return false;
}

StructMessage CommunicationService::ReceiveMessageHost() {
    if (_pipeManager->IsPipeConnected()) {
        return _pipeManager->ReceiveMessage();
    }
    else {
        StructMessage message;
        message.Content = "Error";
        message.MessageSuccessfuly = false;
        return message;
    }
}

void CommunicationService::DeserializeCurrentMachineName(const std::wstring& password) {
    size_t pos = password.find('|');
    _currentMachineName = (password.substr(0, pos));
    _currentPassword = password.substr(pos + 1);
}


std::wstring CommunicationService::CreateConnectionKey() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::wstring caracters = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::wstring password;
    password = Utils::GetCurrentPcName();
    password.push_back(L'|');
    for (int i = 0; i < 6; ++i) {
        std::uniform_int_distribution<int> distribuicao(0, caracters.size() - 1);
        password.push_back(caracters[distribuicao(generator)]);
    }
    return password;
}

bool CommunicationService::OpenCommunicationChannel(const std::wstring& password) {
    if (!_pipeManager->IsPipeConnected()) {
        return _pipeManager->CreatePipeServer(password);
    }
    return true;
}

bool CommunicationService::ConnectChannel(const std::wstring& password)
{
    if (!_pipeManager->IsPipeConnected()) {
        return _pipeManager->CreateClientConnection(password);
    }
    return true;
}

void CommunicationService::SetPipePassword(const std::wstring& password)
{
    _pipeManager->SetPipePassword(password);
}

void CommunicationService::SetClientSide()
{
    _isCurrentServer = false;
}

bool CommunicationService::SendMessageAsClient(const StructMessage& message)
{
    return _pipeManager->SendMessageW(message);
}

StructMessage CommunicationService::ReceiveMessageClient()
{
    return _pipeManager->ReceiveMessage();
}

