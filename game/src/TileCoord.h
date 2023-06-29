
#pragma once

#include "raylib.h"
#include <functional>

struct TileCoord
{
    int x;
    int y;
    float costToReach;  // Add costToReach member
    TileCoord* startNode = nullptr;    // Replace nullptr with a pointer to your desired start node
    TileCoord* goalNode = nullptr;     // Replace nullptr with a pointer to your desired goal node
    TileCoord* currentNode = nullptr;
    float heuristic;
    float totalCost;
    TileCoord* previousNode;
    // Setter function for previousNode
    void setPreviousNode(TileCoord* prevNode) {
        previousNode = prevNode;
    }

    TileCoord() : x(0), y(0), costToReach(0.0f), previousNode(nullptr), startNode(nullptr), goalNode(nullptr), currentNode(nullptr) {}
    TileCoord(int ax, int ay) : x(ax), y(ay), costToReach(0.0f), previousNode(nullptr), startNode(nullptr), goalNode(nullptr) {}
    TileCoord(Vector2 position) : x(position.x), y(position.y), costToReach(0.0f), previousNode(nullptr), startNode(nullptr), goalNode(nullptr) {}

    Vector2 toVec2() const { return Vector2{ static_cast<float>(x), static_cast<float>(y) }; }

    void operator+=(const TileCoord& r) { x += r.x; y += r.y; }
    void operator-=(const TileCoord& r) { x -= r.x; y -= r.y; }
};


bool operator!=(const TileCoord& l, const TileCoord& r) { return l.x != r.x || l.y != r.y; }
bool operator==(const TileCoord& l, const TileCoord& r) { return l.x == r.x && l.y == r.y; }

TileCoord operator+(const TileCoord& l, const TileCoord& r) { return { l.x + r.x, l.y + r.y }; }
TileCoord operator-(const TileCoord& l, const TileCoord& r) { return { l.x - r.x, l.y - r.y }; }

TileCoord operator*(const TileCoord& l, int r) { return { l.x * r, l.y * r }; }
TileCoord operator/(const TileCoord& l, int r) { return { l.x / r, l.y / r }; }

template <> struct std::hash<TileCoord>
{
    size_t operator()(const TileCoord& pos) const // Hash function which produces a unique number (hash) for each unique TileCoord.
    {
        const static long int halfWidth = sqrt(numeric_limits<int>::max());
        const static long int width = halfWidth * 2;
        long long int x = pos.x;
        long long int y = pos.y;
        size_t hash = ((y + halfWidth) * width) + (x + halfWidth);
        //size_t hash = ((x + y) * (x + y + 1) / 2) + y;
        return hash;
    }
};