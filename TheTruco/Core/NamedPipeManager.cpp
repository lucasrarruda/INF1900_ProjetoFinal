#include "pch.h"
#include <NamedPipeManager.h>
#include <NamedPipeConstant.h>
#include <strsafe.h>
#include <AppUtil.h>

using namespace Communication;

NamedPipeManager::NamedPipeManager(const std::wstring& password, const std::wstring& machineName) : _hPipe(INVALID_HANDLE_VALUE), _pipePassword(password) {
    _pipeName = NAMED_PIPE_SERVICE;
}

NamedPipeManager::~NamedPipeManager() {
    if (_hPipe != INVALID_HANDLE_VALUE) {
        CloseHandle(_hPipe);
    }
}

bool NamedPipeManager::IsPipeConnected() const {
    return _connectedToPipe;
}

bool NamedPipeManager::ConnectToPipe(const std::wstring& password) {
    if (_connectedToPipe == false){
        if (password == _pipePassword) {
            _connectedToPipe = ConnectNamedPipe(_hPipe, nullptr) ?
                TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
        }
        else {
            CreateConnectionToPipe(password);
        }
    }
    return _connectedToPipe;

}

bool NamedPipeManager::CreateConnectionToPipe(const std::wstring& password) {

    _hPipe = CreateNamedPipeW(
		_pipeName,             
		PIPE_ACCESS_DUPLEX,     
		PIPE_TYPE_MESSAGE |   
		PIPE_READMODE_MESSAGE |   
		PIPE_WAIT,             
		4,
		BUFSIZE, 
		BUFSIZE,           
		PIPE_UNLIMITED_INSTANCES,             
		NULL);        

	if (_hPipe == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else {

		_pipePassword = password;
		_connectedToPipe = ConnectToPipe(password);
	}
	return _connectedToPipe;
}

bool NamedPipeManager::SendMessageW(const StructMessage& message) {
	std::string serializedMessage = message.Serialize();
	DWORD dwWritten;

	return WriteFile(_hPipe, serializedMessage.c_str(), serializedMessage.size(), &dwWritten, nullptr) != FALSE;
}

StructMessage NamedPipeManager::ReceiveMessage() {

    HANDLE hHeap = GetProcessHeap();
    auto pchRequest = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE * sizeof(TCHAR));
    auto pchReply = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE * sizeof(TCHAR));

    DWORD cbBytesRead = 0;
    BOOL fSuccess = FALSE;


    if (_hPipe == NULL)
    {
        printf("\nERROR - Pipe Server Failure:\n");
        printf("   InstanceThread got an unexpected NULL value in lpvParam.\n");
        printf("   InstanceThread exitting.\n");
        if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
        if (pchRequest != NULL) HeapFree(hHeap, 0, pchRequest);
    } 

    if (pchRequest == NULL)
    {
        printf("\nERROR - Pipe Server Failure:\n");
        printf("   InstanceThread got an unexpected NULL heap allocation.\n");
        printf("   InstanceThread exitting.\n");
        if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
    }

    if (pchReply == NULL)
    {
        printf("\nERROR - Pipe Server Failure:\n");
        printf("   InstanceThread got an unexpected NULL heap allocation.\n");
        printf("   InstanceThread exitting.\n");
        if (pchRequest != NULL) HeapFree(hHeap, 0, pchRequest);
    }

    printf("InstanceThread created, receiving and processing messages.\n");

    while (1)
    {
        if (_hPipe != 0) {
            fSuccess = ReadFile(
                _hPipe,        // handle to pipe 
                pchRequest,    // buffer to receive data 
                BUFSIZE * sizeof(TCHAR), // size of buffer 
                &cbBytesRead, // number of bytes read 
                nullptr);        // not overlapped I/O 
        }
        

        if (!fSuccess || cbBytesRead == 0)
        {
            if (GetLastError() == ERROR_BROKEN_PIPE)
            {
                std::cout << (TEXT("InstanceThread: client disconnected.\n")) << std::endl;
            }
            else
            {
                std::cout <<  "InstanceThread ReadFile failed, GLE=%d.\n" << GetLastError() << std::endl;
            }
            break;
        }
        if (fSuccess) {

            StructMessage message;
            if (&pchRequest[0] != 0) {
                std::wstring messageReceivedChar(&pchRequest[0]);
                std::string messageReceived(messageReceivedChar.begin(), messageReceivedChar.end());
                message = StructMessage::Deserialize(messageReceived);
                message.MessageSuccessfuly = true;
                return message;
            }
        }
        else {
            StructMessage message;
            message.MessageSuccessfuly = false;
            message.Content = "Error with message received return";
            return message;
        }

	}
}

void NamedPipeManager::CloseConnection() {
    FlushFileBuffers(_hPipe);
    DisconnectNamedPipe(_hPipe);
    CloseHandle(_hPipe);
}