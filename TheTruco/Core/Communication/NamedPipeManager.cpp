#include "pch.h"
#include <iostream>
#include <Communication/NamedPipeManager.h>
#include <Communication/NamedPipeConstant.h>
#include <strsafe.h>
#include <Helpers/Utils.h>
#include <sstream>

using namespace Communication;

NamedPipeManager::NamedPipeManager(const std::wstring& password, const std::wstring& machineName) : _hPipe(INVALID_HANDLE_VALUE), _pipePassword(password) {}

NamedPipeManager::~NamedPipeManager() {
	if (_hPipe != INVALID_HANDLE_VALUE) {
		CloseHandle(_hPipe);
	}
}

bool NamedPipeManager::IsPipeConnected() const {
	return _connectedToPipe;
}

void NamedPipeManager::SetPipePassword(const std::wstring& password)
{
	_pipePassword = password;

	const std::wstring namedPipeService(NAMED_PIPE_SERVICE);
	const std::wstring nameServer = namedPipeService + password;
	
	// Usando std::wstringstream para copiar a std::wstring para um buffer
	std::wstringstream copyString;
	copyString << nameServer;

	// Obtendo uma cópia da string do stream
	std::wstring result = copyString.str();

	// Alocar espaço para um novo buffer e copiar a string para ele
	wchar_t* buffer = new wchar_t[result.length() + 1];
	wcscpy_s(buffer, result.length() + 1, result.c_str());

	// Atribuir _pipeName diretamente ao buffer
	_pipeName = buffer;
}

std::wstring Communication::NamedPipeManager::ConvertStringToWString(const std::string& object)
{
	if (object.empty())
		return std::wstring();

	int size = MultiByteToWideChar(CP_UTF8, 0, &object[0], static_cast<int>(object.size()), 0, 0);

	std::wstring result(size, 0);
	MultiByteToWideChar(CP_UTF8, 0, &object[0], static_cast<int>(object.size()), &result[0], size);

	return result;
}

bool NamedPipeManager::ConnectToPipe(const std::wstring& password) {
	if (_connectedToPipe == false) {
		if (password == _pipePassword) {
			_connectedToPipe = ConnectNamedPipe(_hPipe, nullptr) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
		}
		else {
			CreatePipeServer(password);
		}
	}
	return _connectedToPipe;
}

bool NamedPipeManager::CreatePipeServer(const std::wstring& password) {

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

bool NamedPipeManager::CreateClientConnection(const std::wstring& password) {

	_hPipe = CreateFile(
		_pipeName,      // pipe name 
		GENERIC_READ |  // read and write access 
		GENERIC_WRITE,
		0,              // no sharing 
		NULL,           // default security attributes
		OPEN_EXISTING,  // opens existing pipe 
		0,              // default attributes 
		NULL);          // no template file 

	if (_hPipe != INVALID_HANDLE_VALUE)
	{
		return true;
	}

	return false;
}

bool NamedPipeManager::SendMessageW(const StructMessage& message) {
	std::string serializedMessage = message.Serialize();
	std::wstring result = ConvertStringToWString(serializedMessage);

	wchar_t* pchReply = new wchar_t[result.length() + 1];
	wcscpy_s(pchReply, result.length() + 1, result.c_str());

	DWORD dwWritten;

	return WriteFile(_hPipe, pchReply, BUFSIZE * sizeof(wchar_t), &dwWritten, nullptr) != FALSE;
}

StructMessage NamedPipeManager::ReceiveMessage() 
{
	wchar_t* pchRequest = new wchar_t[BUFSIZE];

	DWORD cbBytesRead = 0;
	BOOL fSuccess = FALSE;

	if (_hPipe == NULL)
	{
		printf("\nERROR - Pipe Server Failure:\n");
		printf("   InstanceThread got an unexpected NULL value in lpvParam.\n");
		printf("   InstanceThread exitting.\n");
		if (pchRequest != NULL) HeapFree(GetProcessHeap(), 0, pchRequest);
	}

	if (pchRequest == NULL)
	{
		printf("\nERROR - Pipe Server Failure:\n");
		printf("   InstanceThread got an unexpected NULL heap allocation.\n");
		printf("   InstanceThread exitting.\n");
		if (pchRequest != NULL) HeapFree(GetProcessHeap(), 0, pchRequest);
	}

	while (1)
	{
		if (_hPipe != 0) {
			fSuccess = ReadFile(
				_hPipe,        // handle to pipe 
				pchRequest,    // buffer to receive data 
				BUFSIZE * sizeof(wchar_t), // size of buffer 
				&cbBytesRead, // number of bytes read 
				NULL);        // not overlapped I/O 

			pchRequest[cbBytesRead / sizeof(wchar_t)] = L'\0';
		}


		if (!fSuccess || cbBytesRead == 0)
		{
			if (GetLastError() == ERROR_BROKEN_PIPE)
			{
				std::cout << (TEXT("InstanceThread: client disconnected.\n")) << std::endl;
			}
			else
			{
				std::cout << "InstanceThread ReadFile failed, GLE=%d.\n" << GetLastError() << std::endl;
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
			message.Content = "Error with message received return!";
			return message;
		}

	}
}

void NamedPipeManager::CloseConnection() {
	FlushFileBuffers(_hPipe);
	DisconnectNamedPipe(_hPipe);
	CloseHandle(_hPipe);
}