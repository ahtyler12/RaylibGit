#include "GameState.h"

GameState::GameState()
{
   _gameSettings.ReadSettings();
   _playerData.LoadData();
}
GameState::~GameState()
{
    _playerData.SaveData();
    _gameSettings.WriteSettings();
}