#pragma once
#include "pch.h"
#include <Helpers/DataEncoderUtils.h>
#include <Helpers/DatabaseUtils.h>
#include <Helpers/Constants/Constants.h>
#include <Helpers/Utils.h>
#include <Windows.h>	
#include <Accctrl.h>
#include <Aclapi.h>

using namespace Helpers;
using namespace Helpers::Enums;
using namespace Helpers::Constants;
using namespace std;

void DatabaseUtils::CreateDatabaseFile()
{
	wstring path = GetProfilePath();
	DataEncoderUtils encoder;
	CharacterEncoding characterEncoding = encoder.GetFileEncoding(path);

	if (ERROR_SHARING_VIOLATION == GetLastError())
	{
		return;
	}

	if (characterEncoding == CharacterEncoding::UNICODE_DEFAULT)
	{
		wstring version;
		if (ERROR_NOT_FOUND == Get(L"Expansion", L"Version", version))
		{
			CreateFileByEncode(path, encoder);
		}
	}
	else if (characterEncoding != CharacterEncoding::TRUCO)
	{
		CreateFileByEncode(path, encoder);
	}
}

void DatabaseUtils::CreateFileByEncode(const std::wstring& path, DataEncoderUtils& encoder)
{
	DeleteFile(path.c_str());
	encoder.CreateFileWithEncode(path, CharacterEncoding::TRUCO);
}

wstring DatabaseUtils::GetProfilePath()
{
	return Utils::GetGameDataDirectory() + DATABASE_FILE;
}

DWORD DatabaseUtils::Set(const wstring& section, const wstring& key, const wstring& value)
{
	CreateDatabaseFile();

	BOOL result = WritePrivateProfileString(section.c_str(), key.c_str(), value.c_str(), GetProfilePath().c_str());

	if (result == 0)
	{
		return GetLastError();
	}

	return ERROR_SUCCESS;
}

DWORD DatabaseUtils::Get(const wstring& section, const wstring& key, wstring& value)
{
	wchar_t sizeBufferValue[32767];
	DWORD result = GetPrivateProfileString(section.c_str(), key.c_str(),
		NULL_STR, sizeBufferValue, 5120, GetProfilePath().c_str());
	value = sizeBufferValue;

	if (value == NULL_STR)
	{
		value = L"";
		return ERROR_NOT_FOUND;
	}

	return ERROR_SUCCESS;
}

DWORD DatabaseUtils::GetAll(const wstring& section, wstring& value)
{
	wchar_t sizeBufferValue[32767];
	auto path = GetProfilePath();
	auto pathWchar = path.c_str();

	GetPrivateProfileSection(section.c_str(), sizeBufferValue, sizeof(sizeBufferValue), pathWchar);
	wchar_t* getKey = sizeBufferValue;

	bool getRegister = false;
	while (*getKey) 
	{		
		if (getKey != EMPTY)
		{
			getRegister = true;
			if (value != EMPTY)
			{
				value = value + L"|" + getKey;
			}
			else
			{
				value = getKey;
			}
		}

		getKey += wcslen(getKey) + 1;
	}

	if (getRegister)
	{
		return ERROR_SUCCESS;
	}
	else
	{
		return ERROR_NOT_FOUND;
	}
}

void DatabaseUtils::Remove(const wstring& section, const wstring& key)
{
	WritePrivateProfileString(section.c_str(), key.c_str(), nullptr, GetProfilePath().c_str());
}

void DatabaseUtils::Remove(const wstring& section)
{
	WritePrivateProfileString(section.c_str(), nullptr, nullptr, GetProfilePath().c_str());
}