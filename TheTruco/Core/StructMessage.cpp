#include "pch.h"
#include "StructMessage.h"

std::string StructMessage::Serialize() const {
	return Content;
}

StructMessage StructMessage::Deserialize(const std::string& data) {
	StructMessage message;
	message.Content = data;
	return message;
}

