#include "raylib.h"
#include "rlImGui.h"
#include <vector>
#include <math.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

bool isMouseButtonPressed = false;

Vector2 Vector2Subtract(Vector2 v1, Vector2 v2)
{
    Vector2 result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    return result;
}

Vector2 Vector2Normalize(Vector2 v)
{
    float magnitude = sqrtf(v.x * v.x + v.y * v.y);
    if (magnitude != 0)
    {
        v.x /= magnitude;
        v.y /= magnitude;
    }
    return v;
}

Vector2 Vector2Scale(Vector2 v, float scalar)
{
    Vector2 result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    return result;
}

Vector2 Vector2Add(Vector2 v1, Vector2 v2)
{
    Vector2 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    return result;
}

class Rigidbody
{
public:
    Vector2 position;
    Vector2 velocity;

    Rigidbody(Vector2 startPos)
        : position(startPos), velocity({ 0, 0 })
    {
    }
};

class Agent
{
public:
    Rigidbody rigidbody;
    float maxSpeed;
    float maxAcceleration;

    Agent(Vector2 startPos, float speed, float acceleration)
        : rigidbody(startPos), maxSpeed(speed), maxAcceleration(acceleration)
    {
    }

    void Update(float dt, const Vector2& target, const Vector2& fleePosition)
    {
        if (isMouseButtonPressed)
        {
            // Calculate the direction vector from the agent's position to the target
            Vector2 direction = Vector2Normalize(Vector2Subtract(target, rigidbody.position));

            // Calculate the desired velocity vector
            Vector2 desiredVelocity = Vector2Scale(direction, maxSpeed);

            // Calculate the change in velocity required
            Vector2 deltaVelocity = Vector2Subtract(desiredVelocity, rigidbody.velocity);

            // Calculate the acceleration vector
            Vector2 acceleration = Vector2Normalize(deltaVelocity);
            acceleration = Vector2Scale(acceleration, maxAcceleration);

            // Update the agent's velocity
            rigidbody.velocity = Vector2Add(rigidbody.velocity, Vector2Scale(acceleration, dt));

            // Check if the current speed is greater than the desired speed
            float currentSpeed = sqrtf(rigidbody.velocity.x * rigidbody.velocity.x + rigidbody.velocity.y * rigidbody.velocity.y);
            if (currentSpeed > maxSpeed)
            {
                rigidbody.velocity = Vector2Normalize(rigidbody.velocity);
                rigidbody.velocity = Vector2Scale(rigidbody.velocity, maxSpeed);
            }
        }
        else
        {
            // Flee from the specified position
            Vector2 fleeVector = Flee(rigidbody.position, rigidbody.velocity, fleePosition, maxAcceleration);

            // Update the agent's velocity based on the fleeing acceleration
            rigidbody.velocity = Vector2Add(rigidbody.velocity, Vector2Scale(fleeVector, dt));

            // Check if the current speed is greater than the maximum speed
            float currentSpeed = sqrtf(rigidbody.velocity.x * rigidbody.velocity.x + rigidbody.velocity.y * rigidbody.velocity.y);
            if (currentSpeed > maxSpeed)
            {
                rigidbody.velocity = Vector2Normalize(rigidbody.velocity);
                rigidbody.velocity = Vector2Scale(rigidbody.velocity, maxSpeed);
            }
        }

        // Update the agent's position based on the velocity
        rigidbody.position = Vector2Add(rigidbody.position, Vector2Scale(rigidbody.velocity, dt));
    }

private:
    Vector2 Flee(const Vector2& agentPosition, const Vector2& agentVelocity, const Vector2& targetPosition, float maxAcceleration)
    {
        // Calculate the direction vector from the agent to the target
        Vector2 direction = Vector2Normalize(Vector2Subtract(targetPosition, agentPosition));

        // Calculate the desired velocity vector
        Vector2 desiredVelocity = Vector2Scale(direction, -maxSpeed);

        // Calculate the change in velocity required
        Vector2 deltaVelocity = Vector2Subtract(desiredVelocity, agentVelocity);

        // Calculate the acceleration vector
        Vector2 acceleration = Vector2Normalize(deltaVelocity);
        acceleration = Vector2Scale(acceleration, maxAcceleration);

        return acceleration;
    }

};

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Purple Square");
    SetTargetFPS(120);
    rlImGuiSetup(true);

    std::vector<Agent> agents;

    agents.emplace_back(Vector2{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }, 200.0f, 200.0f);

    Vector2 target = { 0, 0 };
    Vector2 rectanglePosition = { SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - 25 };
    float dt;

    bool useGUI = false;

    while (!WindowShouldClose())
    {
        dt = GetFrameTime();

        // Update the target position based on mouse position
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            isMouseButtonPressed = true;
            target = GetMousePosition();
        }
        else
        {
            isMouseButtonPressed = false;
        }

        if (useGUI)
        {
            // ...
        }
        else
        {
            for (auto& agent : agents)
            {
                agent.Update(dt, target, rectanglePosition);
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(10, 10);
            for (const auto& agent : agents)
            {
                DrawText(TextFormat("Position: (%.2f, %.2f)", agent.rigidbody.position.x, agent.rigidbody.position.y), 10, 30, 20, BLACK);
                DrawCircleV(agent.rigidbody.position, 20, RED);
            }

            // Draw target as a circle
            DrawCircleV(target, 20, GREEN);

            // Draw the rectangle at the middle right of the window
            DrawRectangle(rectanglePosition.x, rectanglePosition.y, 50, 50, PURPLE);

            EndDrawing();
        }

        // Update the target position based on mouse position
        target = GetMousePosition();
    }

    CloseWindow();
    return 0;
}




