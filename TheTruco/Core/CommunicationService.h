#pragma once
#include "NamedPipeManager.h"

class CommunicationService
{
public:
	CommunicationService(std::wstring password);
	CommunicationService();
	~CommunicationService() = default;
	std::wstring GetCurrentPasswordConnection();
	void SetConnectionPassword(std::wstring& password);
	bool SendDataToPipe(const StructMessage& data);
	StructMessage ReceiveDataFromPipe();

private:
	NamedPipeManager _pipeManager;
	std::wstring CreateConnectionKey();
	std::wstring _currentPassword;
};

