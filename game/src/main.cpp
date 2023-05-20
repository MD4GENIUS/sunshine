#include "raylib.h"
#include "rlImGui.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    SetTargetFPS(120);
    rlImGuiSetup(true);

    Vector2 position = { 0, 0 };
    Vector2 velocity = { 0, 0 };
    float dt = GetFrameTime();

    bool useGUI = false;
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_GRAVE))
            useGUI = !useGUI;

        if (useGUI)
        {
            rlImGuiBegin();

            ImGui::SliderFloat("Velocity X", &velocity.x, -10.0f, 10.0f);
            ImGui::SliderFloat("Velocity Y", &velocity.y, -10.0f, 10.0f);

            rlImGuiEnd();
        }
        else
        {
            // Update position based on velocity
            position.x += velocity.x * dt;
            position.y += velocity.y * dt;

            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(10, 10);
            DrawText(TextFormat("Position: (%.2f, %.2f)", position.x, position.y), 10, 30, 20, BLACK);
            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}



