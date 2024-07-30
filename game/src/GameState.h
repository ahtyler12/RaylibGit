#include "EntityManager.h"
#include "PlayerData.h"
#include "ScreenManager.h"
#include "GameSettings.h"
#pragma once

class GameState
{
    public:
        GameState();
        ~GameState();
        void Run();
        void Update();
        void Draw();

    private:
        std::shared_ptr<PlayerData> _playerData = std::make_shared<PlayerData>();
        std::shared_ptr < GameSettings> _gameSettings = std::make_shared <GameSettings>();
        std::shared_ptr < ScreenManager> _screenManager = std::make_shared < ScreenManager>(_gameSettings);
};

/*
     Jobs of the Game State
Create or Load/Save Player Data
Create or Load/Save Game Settings
Keep track of the any data related to the state of the game. 

*/