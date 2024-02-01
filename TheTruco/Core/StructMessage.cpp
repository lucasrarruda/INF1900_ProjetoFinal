#include "pch.h"
#include "StructMessage.h"

using namespace Communication;

std::string StructMessage::Serialize() const {
	return Content;
}

StructMessage StructMessage::Deserialize(const std::string& data) {
	StructMessage message;
	size_t pos = data.find('|');
	message.MessageSuccessfuly = (std::stoi(data.substr(0, pos))) == 1;
	message.Content = data.substr(pos + 1);
	message.Content = data;
	return message;
}

