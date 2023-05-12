#include "rlImGui.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    SetTargetFPS(60);
    Vector2 ballPosition = { -100.0f, -100.0f };
    Color ballColor = BLACK;
    Color ballcolor2 = DARKBLUE;
    float ballRadius = 40.0f;
    float ballRadius2 = 50.0f;
    Vector2 ballposition2 = { 625.0f, 325.0f };


    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        ballPosition = GetMousePosition(); //This function returns a Vector2 containing the current position of the mouse cursor.

        if (CheckCollisionCircles(ballPosition, ballRadius, ballposition2, ballRadius2)) //This function checks if two circles defined by their positions and radii are colliding with each other.
        {
            ballColor = RED; 
            ballcolor2 = VIOLET; 
        }
        else
        {
            ballColor = BLACK; 
            ballcolor2 = DARKBLUE; 
        }

       

        DrawLine(50, 100, 150, 200, BLACK); //This function draws a line with the given start and end points and color.
        DrawCircle(625, 325, 50, DARKBLUE); //This function draws a circle with the given center position, radius, and color.


        DrawText("Hello World!", 16, 9, 20, RED);
        DrawCircleV(ballposition2, ballRadius2, ballcolor2); //This function draws a circle with the given center position, radius, and color.
        DrawCircleV(ballPosition, ballRadius, ballColor); //This function draws a circle with the given position (not fixed position), radius, and color.

        DrawTriangle(Vector2{ 200, 200 }, Vector2{ 250, 250 }, Vector2{ 300, 200 }, PURPLE); //This function draws a triangle with the specified three vertex positions and fill color
        DrawRectangle(400, 300, 50, 100, LIME); //This function draws a rectangle with the specified position, size, and fill color
        DrawRectangleGradientH(600, 200, 100, 50, RED, YELLOW); //This function draws a rectangle with the specified position, size, and horizontal color gradient
        DrawPoly(Vector2{ 900, 100 }, 6, 40, 0, MAROON); //This function draws a regular polygon with the specified position, number of sides, radius, rotation angle, and fill color

        EndDrawing();
    }

    CloseWindow();
    return 0;
}