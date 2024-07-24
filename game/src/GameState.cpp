#include "GameState.h"

GameState::GameState()
{
    playerData.LoadData();
}
GameState::~GameState()
{
    playerData.SaveData();
}