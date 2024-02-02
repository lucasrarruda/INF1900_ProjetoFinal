#pragma once
#include <Helpers/DataEncoderUtils.h>
#include <Windows.h>	
#include <string>

namespace Helpers
{
	class DatabaseUtils
	{
	public:
		static DWORD Set(const std::wstring& section, const std::wstring& key, const std::wstring& value);

		static DWORD Get(const std::wstring& section, const std::wstring& key, std::wstring& value);

		static DWORD GetAll(const std::wstring& section, std::wstring& value);

		static void Remove(const std::wstring& section, const std::wstring& key);

		static void Remove(const std::wstring& section);

	private:
		DatabaseUtils() = default;
		~DatabaseUtils() = default;

		static void CreateDatabaseFile();
		static void CreateFileByEncode(const std::wstring& path, DataEncoderUtils& encoder);
		static std::wstring GetProfilePath();
	};
}