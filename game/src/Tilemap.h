#pragma once
#include <vector>
#include "raylib.h"
#include "TileCoord.h"

#define TILEMAP_WIDTH 16
#define TILEMAP_HEIGHT 9

const TileCoord NORTH = { 0, -1 };
const TileCoord SOUTH = { 0,  1 };
const TileCoord EAST = { 1,  0 };
const TileCoord WEST = { -1,  0 };

enum class Tile
{
	Floor = 0, // Floor tiles can be walked on
	Wall, // Wall tiles cannot be walked on
	Count // number of Tile types (i.e. 2)
};

class Tilemap
{
public:
	int tileSizeX = 100; // width of tiles in pixels when drawing
	int tileSizeY = 100; // height of tiles in pixels when drawing

private:
	//2D array of tiles (just IDs storing which type it is)
	//outer array is to pick a column, inner array is to pick a row from the column,
	//so to get to tile at xy position (0,6) access tiles[0][6]
	Tile tiles[TILEMAP_WIDTH][TILEMAP_HEIGHT];

public:
	size_t GetWidth() // get number of columns in the grid
	{
		return TILEMAP_WIDTH;
	}

	size_t GetHeight()  // get number of rows in the grid
	{
		return TILEMAP_HEIGHT;
	}

	Tile GetTile(TileCoord tilePos); // get the tile at the specified coordinate in the grid

	void SetTile(TileCoord tilePos, Tile value);  // set the tile at the specified coordinate in the grid

	bool ContainsTile(TileCoord tilePosition); // returns true if the coordinate is inside the grid, false otherwise

	bool IsTraversableAt(TileCoord tilePosition); // Returns true if the specified tile is in the level and walkable, false otherwise

	int GetCostForTile(TileCoord tilePositon)  // having this function makes it easier to change costs per tile the future
	{
		return 1;
	}

	Vector2 GetScreenPosOfTileCentered(TileCoord tilePos); //Convert from a tile coordinate to a screen position in middle of tile

	Vector2 GetScreenPosOfTile(TileCoord tilePosition); //Convert from a tile coordinate to a screen position at top left corner of tile

	TileCoord GetTileAtScreenPos(Vector2 positionOnScreen); //Find a tile coordinate given a position on the screen over a tile

	std::vector<TileCoord> GetAllTiles(); // return all tile positions

	std::vector<TileCoord> GetAllTraversableTiles(); // Get all Tiles which are Traversable (in the level and walkable)

	std::vector<TileCoord> GetTraversableTilesAdjacentTo(TileCoord tilePos); // For a given TileCoord, return all TileCoords which are adjacent and are traversable

	/////////////////////////////////////////////////////////////////////////////////////
	// Drawing functions ////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	void DrawTiles(); // Draw all tiles

	void DrawBorders(Color color = BLACK); // Draw lines between tiles

	void DrawAdjacencies(); // Draw lines indicating which tiles are connected to which others

	void DrawCoordinates(); // Write the TileCoordinate on each tile
};