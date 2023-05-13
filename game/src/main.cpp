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

    //Initialize audio device
    InitAudioDevice();

    //Load audio file
    Sound dingSound = LoadSound("../game/assets/audio/ding.mp3");//This function loads the sound from the specific file path

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        Texture2D githubTexture = LoadTexture("../game/assets/textures/github.png");// This function loads a texture image from a file and returns a Texture2D object

        ballPosition = GetMousePosition(); //This function returns a Vector2 containing the current position of the mouse cursor.

        if (CheckCollisionCircles(ballPosition, ballRadius, ballposition2, ballRadius2)) //This function checks if two circles defined by their positions and radii are colliding with each other.
        {
            ballColor = RED; 
            ballcolor2 = VIOLET; 

            // Play audio file
            PlaySound(dingSound);//This function specifies when to play the sound
        }
        else
        {
            ballColor = BLACK; 
            ballcolor2 = DARKBLUE; 
        }

       

        DrawLine(50, 100, 150, 200, BLACK);//This function draws a line with the given start and end points and color.
        DrawCircle(625, 325, 50, DARKBLUE);//This function draws a circle with the given center position, radius, and color.


        DrawText("Welcome to my Goofy World!", 16, 9, 20, RED);
        DrawCircleV(ballposition2, ballRadius2, ballcolor2); //This function draws a circle with the given center position, radius, and color.
        DrawCircleV(ballPosition, ballRadius, ballColor); //This function draws a circle with the given position (not fixed position), radius, and color.

        DrawRectangle(400, 300, 50, 100, LIME); //This function draws a rectangle with the specified position, size, and fill color
        DrawRectangleGradientH(600, 200, 100, 50, RED, YELLOW); //This function draws a rectangle with the specified position, size, and horizontal color gradient
        DrawPoly(Vector2{ 900, 100 }, 6, 40, 0, MAROON); //This function draws a regular polygon with the specified position, number of sides, radius, rotation angle, and fill color

        DrawTexture(githubTexture, SCREEN_WIDTH - githubTexture.width / 1, SCREEN_HEIGHT - githubTexture.height / 1, WHITE); //This function specifies the location of the image file

        EndDrawing();
    }

    CloseWindow();
    return 0;
}