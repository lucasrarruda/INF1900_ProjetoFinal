#pragma once

#include <string>
#include <atlstr.h>

namespace GeneralHelper
{
	inline std::string CStringToString(CString content)
	{
		const char* convertedContent = CT2CA(content);
		return std::string(convertedContent);
	}
};

