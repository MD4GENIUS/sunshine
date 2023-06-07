#include "raylib.h"
#include "rlImGui.h"
#include <vector>
#include <cmath>
#include "raymath.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define WHISKER_LENGTH 100
#define WHISKER_ANGLE 30

class Agent
{
public:
    Vector2 position;
    float rotation;

    Agent(Vector2 startPos)
        : position(startPos), rotation(0.0f)
    {
    }

    void Update(const Vector2& mousePosition, const Vector2& obstaclePosition)
    {
        Vector2 direction = Vector2Subtract(mousePosition, position);
        float angle = atan2f(direction.y, direction.x);
        rotation = RAD2DEG * angle;

        Vector2 obstacleDirection = Vector2Subtract(obstaclePosition, position);
        float obstacleAngle = atan2f(obstacleDirection.y, obstacleDirection.x);
        float angleDiff = rotation - RAD2DEG * obstacleAngle;

        if (angleDiff > 0)
        {
            rotation += 1.0f;
        }
        else
        {
            rotation -= 1.0f;
        }

        position = mousePosition;
    }


};

class Target
{
public:
    Vector2 position;
    float rotation;
    Vector2 velocity;

    Target(Vector2 startPos)
        : position(startPos), rotation(0.0f), velocity(Vector2Zero())
    {
    }

    void Target::Update(const Agent& agent, const std::vector<Vector2>& obstaclePositions)
    {
        Vector2 direction = Vector2Subtract(agent.position, position);
        direction = Vector2Normalize(direction);

        for (const auto& obstaclePos : obstaclePositions)
        {
            Vector2 obstacleDirection = Vector2Subtract(obstaclePos, position);
            float obstacleDistance = Vector2Length(obstacleDirection);

            if (obstacleDistance < WHISKER_LENGTH)
            {
                Vector2 avoidanceForce = Vector2Normalize(obstacleDirection);
                avoidanceForce = Vector2Scale(avoidanceForce, WHISKER_LENGTH - obstacleDistance);

                direction = Vector2Add(direction, avoidanceForce);
            }
        }

        float speed = 2.0f;
        velocity = Vector2Scale(direction, speed);

        position = Vector2Add(position, velocity);

        float angle = atan2f(direction.y, direction.x);
        rotation = RAD2DEG * angle;
    }


    void DrawWhiskers(const Agent& agent) const
    {
        Vector2 right = { cosf(DEG2RAD * (rotation + WHISKER_ANGLE)), sinf(DEG2RAD * (rotation + WHISKER_ANGLE)) };
        Vector2 left = { cosf(DEG2RAD * (rotation - WHISKER_ANGLE)), sinf(DEG2RAD * (rotation - WHISKER_ANGLE)) };

        Vector2 rightFront = { cosf(DEG2RAD * (rotation + WHISKER_ANGLE + 45)), sinf(DEG2RAD * (rotation + WHISKER_ANGLE + 45)) };
        Vector2 rightBack = { cosf(DEG2RAD * (rotation + WHISKER_ANGLE - 45)), sinf(DEG2RAD * (rotation + WHISKER_ANGLE - 45)) };
        Vector2 leftFront = { cosf(DEG2RAD * (rotation - WHISKER_ANGLE + 45)), sinf(DEG2RAD * (rotation - WHISKER_ANGLE + 45)) };
        Vector2 leftBack = { cosf(DEG2RAD * (rotation - WHISKER_ANGLE - 45)), sinf(DEG2RAD * (rotation - WHISKER_ANGLE - 45)) };

        Vector2 rightEndpoint = Vector2Add(position, Vector2Scale(right, WHISKER_LENGTH));
        Vector2 leftEndpoint = Vector2Add(position, Vector2Scale(left, WHISKER_LENGTH));
        Vector2 rightFrontEndpoint = Vector2Add(position, Vector2Scale(rightFront, WHISKER_LENGTH));
        Vector2 rightBackEndpoint = Vector2Add(position, Vector2Scale(rightBack, WHISKER_LENGTH));
        Vector2 leftFrontEndpoint = Vector2Add(position, Vector2Scale(leftFront, WHISKER_LENGTH));
        Vector2 leftBackEndpoint = Vector2Add(position, Vector2Scale(leftBack, WHISKER_LENGTH));

        DrawLineV(position, rightEndpoint, BLACK);
        DrawLineV(position, leftEndpoint, BLACK);
        DrawLineV(position, rightFrontEndpoint, BLACK);
        DrawLineV(position, rightBackEndpoint, BLACK);
        DrawLineV(position, leftFrontEndpoint, BLACK);
        DrawLineV(position, leftBackEndpoint, BLACK);

        bool rightWhiskerCollision = CheckCollisionCircles(position, WHISKER_LENGTH, agent.position, 10.0f);
        bool leftWhiskerCollision = CheckCollisionCircles(position, WHISKER_LENGTH, agent.position, 10.0f);
        bool rightFrontWhiskerCollision = CheckCollisionCircles(position, WHISKER_LENGTH, agent.position, 10.0f);
        bool rightBackWhiskerCollision = CheckCollisionCircles(position, WHISKER_LENGTH, agent.position, 10.0f);
        bool leftFrontWhiskerCollision = CheckCollisionCircles(position, WHISKER_LENGTH, agent.position, 10.0f);
        bool leftBackWhiskerCollision = CheckCollisionCircles(position, WHISKER_LENGTH, agent.position, 10.0f);

        if (rightWhiskerCollision)
        {
            DrawLineV(position, rightEndpoint, RED);
        }

        if (leftWhiskerCollision)
        {
            DrawLineV(position, leftEndpoint, RED);
        }

        if (rightFrontWhiskerCollision)
        {
            DrawLineV(position, rightFrontEndpoint, RED);
        }

        if (rightBackWhiskerCollision)
        {
            DrawLineV(position, rightBackEndpoint, RED);
        }

        if (leftFrontWhiskerCollision)
        {
            DrawLineV(position, leftFrontEndpoint, RED);
        }

        if (leftBackWhiskerCollision)
        {
            DrawLineV(position, leftBackEndpoint, RED);
        }
    }

    void Target::ObstacleAvoidance(const Agent& agent, const std::vector<Vector2>& obstaclePositions)
    {
        for (const auto& obstaclePos : obstaclePositions)
        {
            Vector2 obstacleDirection = Vector2Subtract(obstaclePos, position);
            float obstacleDistance = Vector2Length(obstacleDirection);

            if (obstacleDistance < WHISKER_LENGTH)
            {
                Vector2 avoidanceForce = Vector2Normalize(obstacleDirection);
                avoidanceForce = Vector2Scale(avoidanceForce, WHISKER_LENGTH - obstacleDistance);

                velocity = Vector2Add(velocity, avoidanceForce);
            }
        }
    }

};

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My Lab 3");
    SetTargetFPS(60);

    Agent agent({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
    Target target({ 100.0f, 100.0f });

    std::vector<Vector2> obstaclePositions;
    Vector2 obstaclePosition = { 400.0f, 300.0f };

    while (!WindowShouldClose())
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            obstaclePositions.push_back(GetMousePosition());
        }

        Vector2 mousePosition = GetMousePosition();
        agent.Update(mousePosition, obstaclePosition);
        target.Update(agent, obstaclePositions);
        target.ObstacleAvoidance(agent, obstaclePositions);

        BeginDrawing();
        ClearBackground(WHITE);

        DrawCircleV(agent.position, 20, BLUE);
        DrawCircleV(target.position, 10, RED);

        for (const auto& obstaclePos : obstaclePositions)
        {
            DrawCircleV(obstaclePos, 20, GRAY);
        }

        target.DrawWhiskers(agent);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}





