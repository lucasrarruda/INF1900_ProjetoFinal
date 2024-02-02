#pragma once
#include <string>
#include <Helpers/Enums/CharacterEncoding.h>
#include <Windows.h>

namespace Helpers
{
	class DataEncoderUtils
	{
	public:
		DataEncoderUtils() = default;
		~DataEncoderUtils() = default;

		Helpers::Enums::CharacterEncoding GetFileEncoding(const std::wstring& filePath) const;

		void CreateFileWithEncode(const std::wstring& filePath,
			const Helpers::Enums::CharacterEncoding& encode);
	private:
		PSECURITY_DESCRIPTOR GetSecurityDescriptor();
	};
}