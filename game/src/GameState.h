#include "EntityManager.h"
#include "PlayerData.h"
#pragma once

class GameState
{
    public:
    GameState();
    ~GameState();

    private:
    PlayerData playerData;

};

/*
     Jobs of the Game State
Create the Entity Manager
Create or Load/Save Player Data
Keep track of the any data related to the state of the game. In the current case we only need to know how many entities we have and their screen position.
Later their health/meter, number of rounds won, number of matches won, player data etc will also need to be tracked.

*/