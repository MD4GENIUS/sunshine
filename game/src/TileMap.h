#pragma once
#include "TileCoord.h"
#include <vector>
#include "raylib.h"
#include <string>
#include <game/src/main.cpp>

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
