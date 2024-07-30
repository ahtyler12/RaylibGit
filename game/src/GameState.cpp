#include "GameState.h"

GameState::GameState()
{
   _gameSettings->LoadSettings();
   _playerData->LoadData();
   _screenManager->ChangeScreen(LOGO);
   Run();
}

GameState::~GameState()
{
    _playerData->SaveData();
    _gameSettings->WriteSettings();
}

void GameState::Run()
{
    SetTargetFPS(60); //This should never change
    while (!WindowShouldClose())
    {
        Update();
        Draw();
    }
}

void GameState::Update()
{
}

void GameState::Draw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    _screenManager->DrawScreen();
    EndDrawing();
}
