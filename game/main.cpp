#include "raylib.h"
#include "GameState.h"


int main(void)
{
    SetTargetFPS(60); //This should never change
	std::shared_ptr<GameState> gs = std::make_shared<GameState>(); //Using Shared pointer to make memory management easier

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    
    CloseWindow();
	return 0;
}