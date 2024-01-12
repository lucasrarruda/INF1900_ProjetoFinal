#pragma once
#include <string>
#include <windows.h>
#include "StructMessage.h"

class NamedPipeManager {
public:
	explicit NamedPipeManager(const std::wstring& password);
	~NamedPipeManager();

	bool ConnectToPipe(const std::wstring& password);
	bool SendMessageW(const StructMessage& message);
	StructMessage ReceiveMessage();

private:
	HANDLE hPipe;
	std::wstring _pipeName;
	std::wstring _pipePassword;
};