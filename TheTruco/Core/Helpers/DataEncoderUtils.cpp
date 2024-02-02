#pragma once
#include "pch.h"
#include <Helpers/DataEncoderUtils.h>
#include <Helpers/Constants/Constants.h>
#include <fstream>
#include <string>
#include <sstream>
#include <sddl.h>

using namespace Helpers;
using namespace Helpers::Enums;
using namespace Helpers::Constants;

CharacterEncoding DataEncoderUtils::GetFileEncoding(const std::wstring& filePath) const
{
	CharacterEncoding encoding = CharacterEncoding::NONE;

	DWORD bytesRead = 0;
	HANDLE file = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (file == INVALID_HANDLE_VALUE)
	{
		return encoding;
	}

	BYTE fileHeaderByte[4] = { 0 };
	ReadFile(file, fileHeaderByte, 4, &bytesRead, nullptr);
	CloseHandle(file);

	if (memcmp(fileHeaderByte, BYTE_ORDER_MARK_SETTINGS, sizeof(BYTE_ORDER_MARK_SETTINGS)) == 0)
	{
		encoding = CharacterEncoding::TRUCO;
	}
	else if (memcmp(fileHeaderByte, BYTE_ORDER_MARK_UNICODE, sizeof(BYTE_ORDER_MARK_UNICODE)) == 0)
	{
		encoding = CharacterEncoding::UNICODE_DEFAULT;
	}
	else if (memcmp(fileHeaderByte, BYTE_ORDER_MARK_UNICODE_BE, sizeof(BYTE_ORDER_MARK_UNICODE_BE)) == 0)
	{
		encoding = CharacterEncoding::UNICODE_BIG_ENDIAN;
	}
	else if (memcmp(fileHeaderByte, BYTE_ORDER_MARK_UTF8, sizeof(BYTE_ORDER_MARK_UTF8)) == 0)
	{
		encoding = CharacterEncoding::UTF8_WITH_BYTE_ORDER_MARK;
	}
	else
	{
		encoding = CharacterEncoding::ANSI;
	}

	return encoding;
}

void DataEncoderUtils::CreateFileWithEncode(const std::wstring& filePath, const CharacterEncoding& encode)
{
	SECURITY_ATTRIBUTES securityAttributes{ sizeof(SECURITY_ATTRIBUTES) };
	securityAttributes.lpSecurityDescriptor = GetSecurityDescriptor();

	HANDLE file = CreateFile(filePath.c_str(), GENERIC_ALL, 0, &securityAttributes,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (file == INVALID_HANDLE_VALUE)
	{
		return;
	}

	switch (encode)
	{
		case CharacterEncoding::UNICODE_DEFAULT:
			WriteFile(file, UNICODE_DEFAULT_BUFFER, NUMBER_BYTES_WRITE_TWO, nullptr, nullptr);
			break;
		case CharacterEncoding::UNICODE_BIG_ENDIAN:
			WriteFile(file, UNICODE_BIG_ENDIAN_BUFFER, NUMBER_BYTES_WRITE_TWO, nullptr, nullptr);
			break;
		case CharacterEncoding::UTF8_WITH_BYTE_ORDER_MARK:
			WriteFile(file, UTF8_WITH_BYTE_ORDER_MARK_BUFFER, NUMBER_BYTES_WRITE_THREE, nullptr, nullptr);
			break;
		case CharacterEncoding::TRUCO:
			WriteFile(file, TRUCO_BUFFER, NUMBER_BYTES_WRITE_FOUR, nullptr, nullptr);
			break;
	}

	CloseHandle(file);
}

PSECURITY_DESCRIPTOR DataEncoderUtils::GetSecurityDescriptor()
{
	PSECURITY_DESCRIPTOR securityDescriptor = nullptr;

	if (ConvertStringSecurityDescriptorToSecurityDescriptor(L"D:(A;OICI;GA;;;AU)",
		SECURITY_DESCRIPTOR_REVISION, &securityDescriptor, nullptr) == TRUE)
	{
		return securityDescriptor;
	}

	return nullptr;
}