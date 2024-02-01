#pragma once
#include <StructMessage.h>
#include <windows.h>
#include <string>

#define BUFSIZE 512

namespace Communication {

	class NamedPipeManager {
	public:
		explicit NamedPipeManager(const std::wstring& password, const std::wstring& machineId);
		~NamedPipeManager();
		bool ConnectToPipe(const std::wstring& password);
		bool CreateConnectionToPipe(const std::wstring& password);
		bool SendMessageW(const StructMessage& message);
		void CloseConnection();
		StructMessage ReceiveMessage();
		bool IsPipeConnected() const;

	private:
		HANDLE _hPipe = INVALID_HANDLE_VALUE;
		LPCTSTR _pipeName;
		std::wstring _pipePassword;
		bool _connectedToPipe = false;
	};
}