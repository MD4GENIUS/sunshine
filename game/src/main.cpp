#include <raylib.h>
#include <vector>
#include <cstdlib>
#include "TileCoord.h"

enum class TileType {
    Floor,
    Wall
};

// Tilemap class
class Tilemap {
public:
    // Constructor
    Tilemap(int width, int height) : width_(width), height_(height) {
        // Initialize the tilemap with floor tiles
        tiles_.resize(height_, std::vector<TileType>(width_, TileType::Floor));
    }

    // Get tile at specific position
    TileType getTile(int x, int y) const {
        return tiles_[y][x];
    }

    // Set tile at specific position
    void setTile(int x, int y, TileType tileType) {
        tiles_[y][x] = tileType;
    }

    // Get the width of the tilemap
    int getWidth() const {
        return width_;
    }

    // Get the height of the tilemap
    int getHeight() const {
        return height_;
    }

    // Generate a random level
    void generateRandomLevel() {
        for (int y = 0; y < height_; ++y) {
            for (int x = 0; x < width_; ++x) {
                // Generate a random number between 0 and 99
                int randomValue = rand() % 100;

                // Assign tile type based on the random value
                if (randomValue < 20) {
                    tiles_[y][x] = TileType::Wall;
                }
                else {
                    tiles_[y][x] = TileType::Floor;
                }
            }
        }
    }

    // Check if a tile is traversable
    bool isTileTraversable(int x, int y) const {
        return (tiles_[y][x] == TileType::Floor);
    }

    // Get adjacent traversable tiles for a given tile position
    std::vector<TileCoord> getAdjacentTiles(int x, int y) const {
        std::vector<TileCoord> adjacentTiles;

        // Check adjacent tiles in all four directions
        // NORTH
        if (y > 0 && isTileTraversable(x, y - 1)) {
            adjacentTiles.emplace_back(x, y - 1);
        }

        // SOUTH
        if (y < height_ - 1 && isTileTraversable(x, y + 1)) {
            adjacentTiles.emplace_back(x, y + 1);
        }

        // WEST
        if (x > 0 && isTileTraversable(x - 1, y)) {
            adjacentTiles.emplace_back(x - 1, y);
        }

        // EAST
        if (x < width_ - 1 && isTileTraversable(x + 1, y)) {
            adjacentTiles.emplace_back(x + 1, y);
        }

        return adjacentTiles;
    }

private:
    int width_;                            // Width of the tilemap
    int height_;                           // Height of the tilemap
    std::vector<std::vector<TileType>> tiles_;  // 2D vector to store tile data
};

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Tilemap ");

    const int MAP_WIDTH = 16;
    const int MAP_HEIGHT = 9;
    const float TILE_SIZE = 50.0f;

    Tilemap tilemap(MAP_WIDTH, MAP_HEIGHT);

    // Generate a random level
    tilemap.generateRandomLevel();

    // Load character sprite texture
    Texture2D characterTexture = LoadTexture("..\\game\\assets\\textures\\mario.png");

    // Initial character position
    Vector2 characterPosition = { TILE_SIZE / 2, TILE_SIZE / 2 };

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Update
        Vector2 previousPosition = characterPosition;

        if (IsKeyDown(KEY_W) && characterPosition.y > TILE_SIZE / 2 && tilemap.isTileTraversable(characterPosition.x / TILE_SIZE, (characterPosition.y - TILE_SIZE) / TILE_SIZE)) {
            characterPosition.y -= TILE_SIZE;
        }
        if (IsKeyDown(KEY_S) && characterPosition.y < MAP_HEIGHT * TILE_SIZE - TILE_SIZE / 2 && tilemap.isTileTraversable(characterPosition.x / TILE_SIZE, (characterPosition.y + TILE_SIZE) / TILE_SIZE)) {
            characterPosition.y += TILE_SIZE;
        }
        if (IsKeyDown(KEY_A) && characterPosition.x > TILE_SIZE / 2 && tilemap.isTileTraversable((characterPosition.x - TILE_SIZE) / TILE_SIZE, characterPosition.y / TILE_SIZE)) {
            characterPosition.x -= TILE_SIZE;
        }
        if (IsKeyDown(KEY_D) && characterPosition.x < MAP_WIDTH * TILE_SIZE - TILE_SIZE / 2 && tilemap.isTileTraversable((characterPosition.x + TILE_SIZE) / TILE_SIZE, characterPosition.y / TILE_SIZE)) {
            characterPosition.x += TILE_SIZE;
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw tilemap
        for (int y = 0; y < tilemap.getHeight(); ++y) {
            for (int x = 0; x < tilemap.getWidth(); ++x) {
                const TileType tileType = tilemap.getTile(x, y);
                const Vector2 position{ x * TILE_SIZE, y * TILE_SIZE };
                const Rectangle tileRect = { position.x, position.y, TILE_SIZE, TILE_SIZE };

                // Draw tile
                switch (tileType) {
                case TileType::Floor:
                    DrawRectangleRec(tileRect, BLUE);
                    DrawRectangleLinesEx(tileRect, 1, DARKGRAY);
                    break;
                case TileType::Wall:
                    DrawRectangleRec(tileRect, RED);
                    DrawRectangleLinesEx(tileRect, 1, ORANGE);
                    break;
                }


                // Draw circle in the center of the green lines
                if (tileType == TileType::Floor) {
                    std::vector<TileCoord> adjacentTiles = tilemap.getAdjacentTiles(x, y);
                    Vector2 center = { position.x + TILE_SIZE / 2, position.y + TILE_SIZE / 2 };

                    // Draw lines to adjacent traversable tiles
                    for (const auto& adjacentTile : adjacentTiles) {
                        Vector2 adjacentCenter = { adjacentTile.x * TILE_SIZE + TILE_SIZE / 2, adjacentTile.y * TILE_SIZE + TILE_SIZE / 2 };
                        DrawLine(center.x, center.y, adjacentCenter.x, adjacentCenter.y, GREEN);
                    }

                    // Calculate radius of the circle
                    float radius = TILE_SIZE / 4;

                    // Draw circle in the center of the green lines
                    DrawCircle(center.x, center.y, radius, RED);
                }


            }
        }


        // Draw character sprite
        DrawTexture(characterTexture, characterPosition.x - characterTexture.width / 2, characterPosition.y - characterTexture.height / 2, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
