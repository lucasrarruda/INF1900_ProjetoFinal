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
};

