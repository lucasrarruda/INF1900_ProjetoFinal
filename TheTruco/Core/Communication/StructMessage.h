#pragma once
#include <string>

namespace Communication {
	struct StructMessage
	{
		std::string Content;
		bool MessageSuccessfuly = false;
		std::string Serialize() const;
		static StructMessage Deserialize(const std::string& data);
	};
}