#pragma once
#include <string>
#include <Repository/GameRepository.h>
#include <GameModel.h>
#include <PlayerModel.h>

namespace Service
{
    class GameService
    {
    public:
        GameService() = default;
        explicit GameService(const Repository::GameRepository& gameRepository) : _gameRepository(gameRepository) {};
        ~GameService() = default;

        std::vector<Model::GameModel> GetAllGames();
        Model::GameModel GetGameById(const std::string& id);
        Model::GameModel SaveGame(const Model::GameModel& game);
        void UpdateGame(const Model::GameModel& game);
        void RemoveGame(const Model::GameModel& game);
        Model::GameModel GetConflictingGame(Model::GameModel game);

        Model::GameModel NewGame(std::shared_ptr<Model::PlayerModel>& playerHost);
        Model::GameModel JoinGame(const std::string& id, std::shared_ptr<Model::PlayerModel>& player);
        Model::GameModel RecoverLastGame(const std::string& currentGameID);
        void ReadyGame(Model::GameModel& currentGame, const std::string& nickName);
        
        void StartGame(Model::GameModel& currentGame);
        void LeaveGame(Model::GameModel& currentGame, const std::string& nickName);
        void SurrenderGame(Model::GameModel& currentGame);
      
        void Hand(Model::GameModel& currentGame);
        void PlayCard(Model::GameModel& currentGame, const std::string& nickName, const int& cardKey);
        void HideCard(Model::GameModel& currentGame, const std::string& nickName, const int& cardKey);
        void Truco(Model::GameModel& currentGame);
        bool ElevenHand(Model::GameModel& currentGame, const std::string& nickName);
        bool IronHand(Model::GameModel& currentGame);
        void RunGame(Model::GameModel& currentGame, const std::string& nickName);
        void FinishedRound(Model::GameModel& currentGame);
        void FinishedHand(Model::GameModel& currentGame);

    private:
        Repository::GameRepository _gameRepository;

        Repository::DTOs::GameDTO ToGameDTO(Model::GameModel gameModel);
        Model::GameModel ToGameModel(Repository::DTOs::GameDTO gameDTO);

        void DistributeCards(Model::GameModel& currentGame);
        void TurnCard(Model::GameModel& currentGame);
        bool ElevenHand(const int& totalPoints);
    };
}