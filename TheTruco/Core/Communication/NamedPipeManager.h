#pragma once
#include <Communication/StructMessage.h>
#include <windows.h>
#include <string>

#define BUFSIZE 1024

namespace Communication {

	class NamedPipeManager {
	public:
		explicit NamedPipeManager(const std::wstring& password, const std::wstring& machineId);
		~NamedPipeManager();
		bool ConnectToPipe(const std::wstring& password);
		bool CreatePipeServer(const std::wstring& password);
		bool CreateClientConnection(const std::wstring& password);
		bool SendMessageW(const StructMessage& message);
		void CloseConnection();
		StructMessage ReceiveMessage();
		bool IsPipeConnected() const;
		void SetPipePassword(const std::wstring& password);

	private:
		HANDLE _hPipe = INVALID_HANDLE_VALUE;
		LPCTSTR _pipeName = L"";
		std::wstring _pipePassword;
		bool _connectedToPipe = false;

		std::wstring ConvertStringToWString(const std::string& object);
	};
}