#pragma once
#include <string>
#include <Repository/GameRepository.h>
#include <GameModel.h>
#include <PlayerModel.h>
#include <thread>
#include <future>

namespace Service
{
    class GameService
    {
    public:
        GameService() = default;
        explicit GameService(const Repository::GameRepository& gameRepository) : _gameRepository(gameRepository) {};
        ~GameService() = default;

        void MonitoringPartnerConnection(std::shared_future<bool> isPartnerConnected);

        std::vector<std::shared_ptr<Model::GameModel>> GetAllGames();
        std::shared_ptr<Model::GameModel> GetGameById(const std::string& id);
        std::shared_ptr<Model::GameModel> SaveGame(std::shared_ptr<Model::GameModel> game);
        void UpdateGame(std::shared_ptr<Model::GameModel> game);
        void RemoveGame(std::shared_ptr<Model::GameModel> game);
        std::shared_ptr<Model::GameModel> GetConflictingGame(std::shared_ptr<Model::GameModel> game);

        std::shared_ptr<Model::GameModel> NewGame(std::shared_ptr<Model::PlayerModel>& playerHost);
        std::shared_ptr<Model::GameModel> JoinGame(const std::string& id, std::shared_ptr<Model::PlayerModel>& player);
        std::shared_ptr<Model::GameModel> RecoverLastGame(const std::string& currentGameID);
        void ReadyGame(std::shared_ptr<Model::GameModel>& currentGame, const std::string& nickName);
        
        void StartGame(std::shared_ptr<Model::GameModel>& currentGame);
        void LeaveGame(std::shared_ptr<Model::GameModel>& currentGame, const std::string& nickName);
        void SurrenderGame(std::shared_ptr<Model::GameModel>& currentGame);
      
        void Hand(std::shared_ptr<Model::GameModel>& currentGame);
        void PlayCard(std::shared_ptr<Model::GameModel>& currentGame, const std::string& nickName, const int& cardKey);
        void HideCard(std::shared_ptr<Model::GameModel>& currentGame, const std::string& nickName, const int& cardKey);
        void Truco(std::shared_ptr<Model::GameModel>& currentGame);
        bool ElevenHand(std::shared_ptr<Model::GameModel>& currentGame, const std::string& nickName);
        bool IronHand(std::shared_ptr<Model::GameModel>& currentGame);
        void RunGame(std::shared_ptr<Model::GameModel>& currentGame, const std::string& nickName);
        void FinishedRound(std::shared_ptr<Model::GameModel>& currentGame);
        void FinishedHand(std::shared_ptr<Model::GameModel>& currentGame);

    private:
        std::thread _gameThread;
        Repository::GameRepository _gameRepository;
        std::thread _gameThread;

        Repository::DTOs::GameDTO ToGameDTO(std::shared_ptr<Model::GameModel> gameModel);
        std::shared_ptr<Model::GameModel> ToGameModel(Repository::DTOs::GameDTO gameDTO);

        void DistributeCards(std::shared_ptr<Model::GameModel>& currentGame);
        void TurnCard(std::shared_ptr<Model::GameModel>& currentGame);
        bool ElevenHand(const int& totalPoints);
    };
}