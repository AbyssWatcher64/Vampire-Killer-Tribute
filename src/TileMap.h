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

	BLOCK_WHITE_WALL_DIRT_9, BLOCK_WHITE_WALL_DIRT_10, /*BLOCK_STAIRS_RIGHT_1*/ BLOCK_WHITE_WALL_DIRT_11 = 84,
	/*BLOCK_STAIRS_LEFT_1*/BLOCK_WHITE_WALL_DIRT_12 = 86, BLOCK_WHITE_ARCH_1, BLOCK_WHITE_ARCH_2,
	BLOCK_WHITE_ARCH_3, BLOCK_WHITE_ARCH_4, BLOCK_WHITE_ARCH_5, BLOCK_WHITE_ARCH_6,
	BLOCK_WINDOW, BLOCK_COLUMN_1, BLOCK_COLUMN_2, BLOCK_COLUMN_3,

	BLOCK_COLUMN_4, BLOCK_COLUMN_5, BLOCK_COLUMN_6, BLOCK_COLUMN_7,
	BLOCK_COLUMN_8, BLOCK_CURTAINS_1, BLOCK_CURTAINS_2, BLOCK_CURTAINS_3,
	BLOCK_CURTAINS_4, BLOCK_CURTAINS_5, BLOCK_CURTAINS_6, BLOCK_CURTAINS_7,
	BLOCK_CURTAINS_8, BLOCK_CURTAINS_9, BLOCK_CURTAINS_10, BLOCK_CURTAINS_11,

	BLOCK_BLACK, BLOCK_STONE_1, BLOCK_STONE_2, BLOCK_STONE_3,
	BLOCK_STONE_4, BLOCK_STONE_5, /*BLOCK_STAIRS_RIGHT_2*/ /*BLOCK_STAIRS_LEFT_2*/
	BLOCK_DARK_GATE_1 = 121, BLOCK_DARK_GATE_2, BLOCK_DARK_GATE_3, BLOCK_COLUMN_DIRT_1,
	BLOCK_COLUMN_DIRT_2, BLOCK_STONE_WINDOW, BLOCK_STONE_6, BLOCK_STONE_7,

	BLOCK_STONE_8, BLOCK_MOSS_1, BLOCK_MOSS_2, BLOCK_MOSS_3,
	BLOCK_WATER_1, BLOCK_WATER_2, BLOCK_WATER_3, BLOCK_WATER_4,
	BLOCK_DOOR, BLOCK_GATE_2, BLOCK_TREE_33, BLOCK_TREE_34,
	BLOCK_WALL_OUTSIDE_8, BLOCK_WALL_OUTSIDE_9, BLOCK_WALL_OUTSIDE_10, BLOCK_MOSS_4,

	/*BLOCK_STAIRS_LEFT_3*/ BLOCK_WHITE_WALL_2 = 145,


	BLOCK_STAIRS_LEFT_1_BOT = 150, BLOCK_STAIRS_LEFT_1, BLOCK_STAIRS_LEFT_1_TOP,
	BLOCK_STAIRS_RIGHT_1_BOT = 160, BLOCK_STAIRS_RIGHT_1, BLOCK_STAIRS_RIGHT_1_TOP,
	BLOCK_STAIRS_LEFT_2_BOT = 170, BLOCK_STAIRS_LEFT_2, BLOCK_STAIRS_LEFT_2_TOP,
	BLOCK_STAIRS_RIGHT_2_BOT = 180, BLOCK_STAIRS_RIGHT_2, BLOCK_STAIRS_RIGHT_2_TOP,
	BLOCK_STAIRS_LEFT_3_BOT = 190, BLOCK_STAIRS_LEFT_3, BLOCK_STAIRS_LEFT_3_TOP,

	PLAYER = 200,
	ZOMBIE = 201,
	BLACKLEOPARD_LEFT = 210, BLACKLEOPARD_RIGHT = 211,

	ITEM_SHIELD = 300, ITEM_MORNING_STAR, ITEM_DAGGER,

	ITEM_WHITEBAG = 400, ITEM_BLUEBAG, ITEM_YELLOWKEY, ITEM_WHITEKEY, ITEM_HEART, ITEM_BIGHEART,

	ITEM_ORB = 500,

	ITEM_CHEST = 600,

	PYRE = 700, CANDLE, BLOCKS, DOOR_OPENABLE,

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
	SOLID_FIRST = BLOCK_GROUND_GRASS, /*BLOCK_PLATFORM_1,*/
	SOLID_LAST = BLOCK_GROUND_GRASS, /*BLOCK_PLATFORM_3,*/
	OBJECT_FIRST = ITEM_SHIELD, //KEY_RED
	OBJECT_LAST = ITEM_ORB, //ITEM_CHILI 
	ENTITY_FIRST = PLAYER,
	ENTITY_LAST = ZOMBIE

};

class TileMap
{
public:
	TileMap();
	~TileMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void ClearObjectEntityPositions();

	void Update();
	void Render();
	void Release();

	bool IsTileObject(Tile tile) const;
	bool IsTileEntity(Tile tile) const;

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
	bool TestOnLadderLeft(const AABB& box, int* px) const;
	
	//Test if box is on ladder top and update 'px' with the x-center position of the ladder
	bool TestOnLadderTopRight(const AABB& box, int* px) const;
	bool TestOnLadderTopLeft(const AABB& box, int* px) const;
	bool TestOnLadderTop(const AABB& box, int* px) const;

	bool TestOnLadderBotRight(const AABB& box, int* px) const;
	bool TestOnLadderBotLeft(const AABB& box, int* px) const;

	//Given a hitbox, computes the maximum swept box model along the X-axis without solid tiles
	AABB GetSweptAreaX(const AABB& hitboxbox) const;

private:
	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	bool IsTileSolid(Tile tile) const;
	bool IsTileLadderTop(Tile tile) const;
	bool IsTileLadderTopLeft(Tile tile) const;
	bool IsTileLadderTopRight(Tile tile) const;
	bool IsTileLadder(Tile tile) const;
	bool IsTileLadderBotLeft(Tile tile) const;
	bool IsTileLadderBotRight(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;
	int GetLadderCenterPos(int pixel_x, int pixel_y) const;

	//Tile map
	Tile *map;
	//Tile  **map;
	//std::vector<Tile*> screens; // either this or line above are the same

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

