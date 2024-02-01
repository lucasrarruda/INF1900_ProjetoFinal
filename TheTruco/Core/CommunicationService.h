#pragma once
#include <NamedPipeManager.h>
#include <AppUtil.h>
#include <memory>
#include <string>

namespace Communication {

	class CommunicationService
	{
	public:
		explicit CommunicationService(const std::wstring& password);
		CommunicationService();
		~CommunicationService() = default;
		std::wstring GetConnectionKey() const;
		void SetConnectionPassword(const std::wstring_view& password);
		bool SendDataToPipe(const StructMessage& data);
		StructMessage ReceiveDataFromPipe();
		bool OpenCommunicationChannel(const std::wstring& password);

	private:
		std::unique_ptr<NamedPipeManager> _pipeManager;
		std::wstring CreateConnectionKey();
		std::wstring _currentPassword;
		std::wstring _currentConnectionKey;
		std::wstring _currentMachineName;
		bool _isCurrentServer = false;
		void DeserializeCurrentMachineName(const std::wstring& password);
	};
}

