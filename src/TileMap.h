#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>

enum class Tile {

	// -1: area covered by an entity
	EMPTY = -1,
	//  0: air tile
	AIR = 0,

	BLOCK_GROUND_GRASS = 1, BLOCK_GATE, BLOCK_GATE_TREE_1, BLOCK_GATE_TREE_2,
	BLOCK_TREE_1, BLOCK_TREE_2, BLOCK_TREE_3, BLOCK_TREE_4,
	BLOCK_TREE_5, BLOCK_TREE_6, BLOCK_TREE_7, BLOCK_TREE_8,
	BLOCK_TREE_9, BLOCK_TREE_10, BLOCK_TREE_11, BLOCK_TREE_12,

	BLOCK_TREE_13, BLOCK_TREE_14, BLOCK_TREE_15, BLOCK_TREE_16,
	BLOCK_TREE_17, BLOCK_TREE_18, BLOCK_TREE_19, BLOCK_TREE_20,
	BLOCK_TREE_21, BLOCK_TREE_22, BLOCK_TREE_23, BLOCK_TREE_24,
	BLOCK_TREE_25, BLOCK_TREE_26, BLOCK_TREE_27, BLOCK_TREE_28,

	BLOCK_TREE_29, BLOCK_TREE_30, BLOCK_TREE_31, BLOCK_TREE_32,
	BLOCK_TREE_WALL, BLOCK_WALL_OUTSIDE_1, BLOCK_WALL_OUTSIDE_2, BLOCK_WALL_OUTSIDE_3,
	BLOCK_WALL_OUTSIDE_4, BLOCK_WALL_OUTSIDE_5, BLOCK_WALL_OUTSIDE_6, BLOCK_WALL_OUTSIDE_7,
	BLOCK_WALL_OUTSIDE_WINDOW_1, BLOCK_WALL_OUTSIDE_WINDOW_2, BLOCK_WALL_OUTSIDE_WINDOW_3, BLOCK_WALL_OUTSIDE_WINDOW_4,

	BLOCK_WALL_OUTSIDE_WINDOW_5, BLOCK_WALL_OUTSIDE_WINDOW_6, BLOCK_WALL_OUTSIDE_WINDOW_7, BLOCK_WALL_OUTSIDE_WINDOW_8,
	BLOCK_WALL_OUTSIDE_WINDOW_9, BLOCK_WALL_OUTSIDE_WINDOW_10, BLOCK_WALL_OUTSIDE_WINDOW_11, BLOCK_WALL_OUTSIDE_WINDOW_12,
	BLOCK_WALL_INSIDE_1, BLOCK_WALL_INSIDE_2, BLOCK_WALL_INSIDE_WINDOW_1, BLOCK_WALL_INSIDE_WINDOW_2,
	BLOCK_WALL_INSIDE_3, BLOCK_COLUMN_STATUE_1, BLOCK_COLUMN_STATUE_2, BLOCK_COLUMN_STATUE_3,

	BLOCK_COLUMN_STATUE_4, BLOCK_COLUMN_STATUE_5, BLOCK_COLUMN_STATUE_6, BLOCK_COLUMN_STATUE_7,
	BLOCK_PLATFORM_1, BLOCK_PLATFORM_2, BLOCK_PLATFORM_3, BLOCK_WHITE_WALL,
	BLOCK_WHITE_WALL_DIRT_1, BLOCK_WHITE_WALL_DIRT_2, BLOCK_WHITE_WALL_DIRT_3, BLOCK_WHITE_WALL_DIRT_4,
	BLOCK_WHITE_WALL_DIRT_5, BLOCK_WHITE_WALL_DIRT_6, BLOCK_WHITE_WALL_DIRT_7, BLOCK_WHITE_WALL_DIRT_8,

	BLOCK_GATE_2, BLOCK_TREE_33, BLOCK_TREE_34, BLOCK_WALL_OUTSIDE_8, BLOCK_WALL_OUTSIDE_9, BLOCK_WALL_OUTSIDE_10,

	//TODO Add the rest of the blocks

	PLAYER = 100,
	ZOMBIE = 101,


	FIRE = 800, FIRE_FRAME0, FIRE_FRAME1,

	//  0: empty tile
	// -1: area covered by entity

	// 0 < id < 50: static tiles
	BLOCK_SQUARE1_TL = 1000, BLOCK_SQUARE1_TR, BLOCK_SQUARE1_BL, BLOCK_SQUARE1_BR,
	BLOCK_SQUARE2_TL, BLOCK_SQUARE2_TR, BLOCK_SQUARE2_BL, BLOCK_SQUARE2_BR,
	BLOCK_VERT2_T, BLOCK_VERT2_B, BLOCK_HORIZ2_L, BLOCK_HORIZ2_R, BLOCK_BLUE,
	BLOCK_HORIZ3_L, BLOCK_HORIZ3_M, BLOCK_HORIZ3_R,
	BLOCK_BEAM_L, BLOCK_BEAM_R,

	LADDER_L = 2000, LADDER_R, LADDER_TOP_L, LADDER_TOP_R,
	LOCK_RED = 3000, LOCK_YELLOW,
	LASER_L = 4000, LASER_R,

	// 50 <= id < 100: special tiles
	DOOR = 5000,
	KEY_RED = 6000, YELLOW_KEY, ITEM_APPLE, ITEM_CHILI,
	LASER = 7000, LASER_FRAME0, LASER_FRAME1, LASER_FRAME2,


	//Intervals
	STATIC_FIRST = BLOCK_SQUARE1_TL,
	STATIC_LAST = LASER_R,
	//SOLID_FIRST = BLOCK_SQUARE1_TL,
	SOLID_FIRST = BLOCK_GROUND_GRASS,
	SOLID_LAST = BLOCK_GROUND_GRASS,
	SPECIAL_FIRST = DOOR,
	SPECIAL_LAST = LASER,
	ENTITY_FIRST = PLAYER,
	ENTITY_LAST = PLAYER

};

class TileMap
{
public:
	TileMap();
	~TileMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void Update();
	void Render();
	void Release();

	//Test for collisions with walls
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;
	
	//Test collision with the ground and update 'py' with the maximum y-position to prevent
	//penetration of the grounded tile, that is, the pixel y-position above the grounded tile.
	//Grounded tile = solid tile (blocks) or ladder tops.
	bool TestCollisionGround(const AABB& box, int *py) const;
	
	//Test if there is a ground tile one pixel below the given box
	bool TestFalling(const AABB& box) const;
	
	//Test if box is on ladder and update 'px' with the x-center position of the ladder
	bool TestOnLadder(const AABB& box, int* px) const;
	
	//Test if box is on ladder top and update 'px' with the x-center position of the ladder
	bool TestOnLadderTop(const AABB& box, int* px) const;

private:
	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	bool IsTileSolid(Tile tile) const;
	bool IsTileLadderTop(Tile tile) const;
	bool IsTileLadder(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;
	int GetLadderCenterPos(int pixel_x, int pixel_y) const;

	//Tile map
	Tile *map;

	//Size of the tile map
	int size, width, height;
	
	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	Sprite *laser;
	Sprite* fire;
	
	//Tile sheet
	const Texture2D *img_tiles;
	const Texture2D *img_items;
};

