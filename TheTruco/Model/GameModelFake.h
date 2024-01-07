#pragma once

#include <ModelBase.h>
#include <string>

class GameModelFake: public ModelBase
{
public:
	GameModelFake() = default;
	~GameModelFake() = default;

	void Attach(std::shared_ptr<ViewBase> observer);
	void Detach(std::shared_ptr<ViewBase> observer);
	void Notify();

	inline void SetGameID(const std::string& id) { _gameID = id; }
	inline std::string GetGameID() { return _gameID; }

private:
	std::string _gameID;
};

