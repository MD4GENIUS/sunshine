#include "rlImGui.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    SetTargetFPS(120);
    rlImGuiSetup(true); //To get ImGUI ready

    bool useGUI = false;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawFPS(10, 10);
        EndDrawing();
    }

    if (IsKeyPressed(KEY_GRAVE)) useGUI = !useGUI;
    if (useGUI)
    {
        rlImGuiBegin(); //to make any ImGUI window appear, start here.
        //then call the relevant functions for each element before calling rlImGuiEnd()



        rlImGuiEnd(); //to make any ImGUI window appear, end with this
    }

    CloseWindow();
    return 0;
}