#pragma once

#include <Interfaces/ModelBase.h>
#include <string>

class GameModelFake: public Interfaces::ModelBase
{
public:
	GameModelFake() = default;
	~GameModelFake() = default;

	inline void SetGameID(const std::string& id) { _gameID = id; }
	inline std::string GetGameID() { return _gameID; }

private:
	std::string _gameID;
};

