#include <vector>
#include <cstdlib>
#include "TileCoord.h"
#include <unordered_map>
#include <queue>
#include <unordered_set>
#include <string>
#include <raylib.h>
#include <algorithm> 
#include <limits>

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

class Pathfinder {
public:
    Pathfinder(const Tilemap& tilemap, const TileCoord& start, const TileCoord& goal)
        : tilemap_(tilemap), start_(start), goal_(goal) {
        const int mapSize = tilemap.getWidth() * tilemap.getHeight();
        bestPrevious_.resize(mapSize, TileCoord(-1, -1));
        costs_.resize(mapSize, std::numeric_limits<int>::max());

    }

    TileCoord getLowestCostNode(const std::unordered_set<TileCoord>& unvisitedSet) {
        TileCoord lowestCostNode = TileCoord(-1, -1);
        int lowestCost = std::numeric_limits<int>::max();
        std::unordered_map<TileCoord, int> costMap;

        for (const auto& tile : unvisitedSet) {
            const int index = tile.y * tilemap_.getWidth() + tile.x;
            costMap[tile] = costs_[index];
        }

        // Find the tile with the lowest cost
        for (const auto& pair : costMap) {
            if (pair.second < lowestCost) {
                lowestCost = pair.second;
                lowestCostNode = pair.first;
            }
        }

        return lowestCostNode;
    }


    void runDijkstraAlgorithm() {
        std::unordered_set<TileCoord> visitedSet;
        std::unordered_set<TileCoord> unvisitedSet;

        // Add all tile coordinates to the unvisited set
        for (int y = 0; y < tilemap_.getHeight(); ++y) {
            for (int x = 0; x < tilemap_.getWidth(); ++x) {
                unvisitedSet.insert(TileCoord(x, y));
            }
        }

        // Initialize costs and best previous nodes
        for (const auto& tile : unvisitedSet) {
            const int index = tile.y * tilemap_.getWidth() + tile.x;
            costs_[index] = std::numeric_limits<int>::max();
            bestPrevious_[index] = TileCoord(-1, -1);
        }

        // Initialize start node
        const int startIndex = start_.y * tilemap_.getWidth() + start_.x;
        costs_[startIndex] = 0;

        while (!unvisitedSet.empty()) {
            // Find the node with the lowest cost in the unvisited set
            TileCoord current = getLowestCostNode(unvisitedSet);

            // Remove the current node from the unvisited set
            unvisitedSet.erase(current);

            // Add the current node to the visited set
            visitedSet.insert(current);

            // Get adjacent tiles
            const std::vector<TileCoord> adjacentTiles = tilemap_.getAdjacentTiles(current.x, current.y);

            for (const auto& adjacent : adjacentTiles) {
                // Skip if the adjacent tile is in the visited set
                if (visitedSet.count(adjacent) > 0) {
                    continue;
                }

                const int currentIndex = current.y * tilemap_.getWidth() + current.x;
                const int adjacentIndex = adjacent.y * tilemap_.getWidth() + adjacent.x;
                const int newCost = costs_[currentIndex] + 1; // Calculate the new cost

                // Update costs and best previous nodes for adjacent tiles
                if (newCost < costs_[adjacentIndex]) {
                    costs_[adjacentIndex] = newCost;
                    bestPrevious_[adjacentIndex] = current;
                }
            }
        }
    }

    void clearCosts() {
        costs_.clear();
    }

    void resizeCosts(int size, int value) {
        costs_.resize(size, value);
    }

    void clearBestPrevious() {
        bestPrevious_.clear();
    }

    void resizeBestPrevious(int size, const TileCoord& value) {
        bestPrevious_.resize(size, value);
    }

    std::vector<TileCoord> getPathToGoal() const {
        std::vector<TileCoord> path;
        TileCoord current = goal_;

        while (current != start_ && current.x >= 0 && current.y >= 0) {
            path.push_back(current);
            const int currentIndex = current.y * tilemap_.getWidth() + current.x;
            current = bestPrevious_[currentIndex];
        }

        // Add the start node
        path.push_back(start_);

        // Reverse the path
        std::reverse(path.begin(), path.end());

        return path;

    }

private:
    const Tilemap& tilemap_;
    const TileCoord start_;
    const TileCoord goal_;
    std::vector<int> costs_;
    std::vector<TileCoord> bestPrevious_;

    int getTileCost(const TileCoord& tile) const {
        // Assign costs based on tile types
        const TileType tileType = tilemap_.getTile(tile.x, tile.y);
        switch (tileType) {
        case TileType::Floor:
            return 1;
        case TileType::Wall:
            return std::numeric_limits<int>::max();  // Wall tiles are not traversable
        }
        return std::numeric_limits<int>::max();  // Unknown tile type, treat it as non-traversable
    }

};

bool startTileSet = false;
bool goalTileSet = false;


int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Mohamed Meghji Tilemap ");

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

    TileCoord startTile(-3, -2);  // Initialize start tile with invalid coordinates
    TileCoord goalTile(-1, -1);   // Initialize goal tile with invalid 

    TileCoord clickedTile;
    bool tileClicked = false;       // Flag to track if a tile is clicked
    bool spacebarPressed = false;  // Flag to track if spacebar is pressed

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

        Pathfinder pathfinder(tilemap, startTile, goalTile);  // Create an instance of Pathfinder

        if (IsKeyPressed(KEY_SPACE)) {

            spacebarPressed = true;

            // Check if start and goal tiles are set
            if (!startTileSet || !goalTileSet) {
                printf("Start and goal positions must be set before running the algorithm!\n");
                spacebarPressed = false;
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Right-click to set start and goal positions
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            // Get tile coordinates under mouse position
            int mouseX = GetMouseX();
            int mouseY = GetMouseY();
            int tileX = static_cast<int>(mouseX / TILE_SIZE);
            int tileY = static_cast<int>(mouseY / TILE_SIZE);

            // Set start position if not already set
            if (!startTileSet) {
                startTile = TileCoord(tileX, tileY);
                startTileSet = true;
                printf("Start position set: (%d, %d)\n", tileX, tileY);
            }
            else if (!goalTileSet) {
                goalTile = TileCoord(tileX, tileY);
                goalTileSet = true;
                printf("Goal position set: (%d, %d)\n", tileX, tileY);
            }
            // Clear clicked tile if clicked
            else if (tilemap.getTile(tileX, tileY) == TileType::Floor) {
                clickedTile = TileCoord(tileX, tileY);
                tileClicked = true;
            }
        }


        // Draw tilemap
        for (int y = 0; y < tilemap.getHeight(); ++y) {
            for (int x = 0; x < tilemap.getWidth(); ++x) {
                const TileType tileType = tilemap.getTile(x, y);
                const Vector2 position{ x * TILE_SIZE, y * TILE_SIZE };
                const Rectangle tileRect = { position.x, position.y, TILE_SIZE, TILE_SIZE };

                // Draw tile
                switch (tileType) {
                case TileType::Floor:
                    DrawRectangleRec(tileRect, WHITE);
                    DrawRectangleLinesEx(tileRect, 1, DARKGRAY);
                    break;
                case TileType::Wall:
                    DrawRectangleRec(tileRect, PURPLE);
                    DrawRectangleLinesEx(tileRect, 1, ORANGE);
                    break;
                }

                // Draw start node
                if (startTileSet && startTile.x == x && startTile.y == y) {
                    const Vector2 startNodePosition = {
                        startTile.x * TILE_SIZE,
                        startTile.y * TILE_SIZE
                    };
                    const Rectangle startNodeRect = { startNodePosition.x, startNodePosition.y, TILE_SIZE, TILE_SIZE };
                    DrawRectangleRec(startNodeRect, GREEN);
                    DrawRectangleLinesEx(startNodeRect, 2, GREEN);
                    DrawText("Start", static_cast<int>(startNodePosition.x + 5), static_cast<int>(startNodePosition.y + 5), 10, WHITE);
                }

                // Draw goal node
                if (goalTileSet && goalTile.x == x && goalTile.y == y) {
                    const Vector2 goalNodePosition = {
                        goalTile.x * TILE_SIZE,
                        goalTile.y * TILE_SIZE
                    };
                    const Rectangle goalNodeRect = { goalNodePosition.x, goalNodePosition.y, TILE_SIZE, TILE_SIZE };
                    DrawRectangleRec(goalNodeRect, RED);
                    DrawRectangleLinesEx(goalNodeRect, 2, RED);
                    DrawText("Goal", static_cast<int>(goalNodePosition.x + 5), static_cast<int>(goalNodePosition.y + 5), 10, WHITE);
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
                }
            }
        }

        // Draw current node
        const TileCoord currentTile = TileCoord(
            static_cast<int>(characterPosition.x / TILE_SIZE),
            static_cast<int>(characterPosition.y / TILE_SIZE)
        );
        const Vector2 currentNodePosition = {
            currentTile.x * TILE_SIZE,
            currentTile.y * TILE_SIZE
        };

        const Rectangle currentNodeRect = { currentNodePosition.x, currentNodePosition.y, TILE_SIZE, TILE_SIZE };
        DrawRectangleLinesEx(currentNodeRect, 2, BLUE);
        DrawText("Current", static_cast<int>(currentNodePosition.x + 5), static_cast<int>(currentNodePosition.y + 5), 10, BLUE);

        if (spacebarPressed && startTileSet && goalTileSet) {
            Pathfinder pathfinder(tilemap, startTile, goalTile);
            pathfinder.runDijkstraAlgorithm();
            std::vector<TileCoord> solution = pathfinder.getPathToGoal();

            // Draw solution in blue
            for (size_t i = 1; i < solution.size(); ++i) {
                const TileCoord& prev = solution[i - 1];
                const TileCoord& current = solution[i];

                Vector2 prevPosition = {
                    prev.x * TILE_SIZE + TILE_SIZE / 2,
                    prev.y * TILE_SIZE + TILE_SIZE / 2
                };
                Vector2 currentPosition = {
                    current.x * TILE_SIZE + TILE_SIZE / 2,
                    current.y * TILE_SIZE + TILE_SIZE / 2
                };

                DrawLineEx(prevPosition, currentPosition, 2, BLUE);

                spacebarPressed = false;
            }
        }

        // Draw character sprite
        DrawTexture(characterTexture, characterPosition.x - characterTexture.width / 2, characterPosition.y - characterTexture.height / 2, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}