#pragma once

#include <string>
#include <atlstr.h>

namespace GeneralHelper
{
	inline std::string CStringToString(CString content)
	{
		CT2CA convertedContent(content);
		return std::string(convertedContent);
	}

	inline CString StringToCString(std::string content)
	{
		CString convertedContent(content.c_str());
		return convertedContent;
	}

	inline std::wstring CStringToWstring(CString content)
	{
		std::wstring convertedContent(content);
		return convertedContent;
	}

	inline CString WstringToCString(std::wstring content)
	{
		CString convertedContent(content.c_str());
		return convertedContent;
	}
};

