#pragma once

#include <string>


struct StructMessage
{
	std::string Content;

	std::string Serialize() const;
	static StructMessage Deserialize(const std::string& data);
};
