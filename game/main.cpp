#include "raylib.h"
#include "GameState.h"


int main(void)
{
    //const int screenWidth = 800;
    //const int screenHeight = 600;
    //SetConfigFlags(FLAG_MSAA_4X_HINT);
    //InitWindow(screenWidth, screenHeight, "My first RAYLIB program!");
    SetTargetFPS(60);
	GameState gs = GameState();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
	return 0;
}