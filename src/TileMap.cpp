#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>

TileMap::TileMap()
{
	map = nullptr;
	width = 0;
	height = 0;
	fire = nullptr;
	laser = nullptr;
	img_tiles = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (map != nullptr)
	{
		delete[] map;
		map = nullptr;
	}
	if (laser != nullptr)
	{
		laser->Release();
		delete laser;
		laser = nullptr;
	}
	if (fire != nullptr)
	{
		fire->Release();
		delete fire;
		fire = nullptr;
	}
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;

	dict_rect[(int)Tile::BLOCK_GROUND_GRASS] = { 0,  0, n, n };
	dict_rect[(int)Tile::BLOCK_GATE] = { n,  0, n, n };
	dict_rect[(int)Tile::BLOCK_GATE_TREE_1] = { 2 * n, 0, n, n };
	dict_rect[(int)Tile::BLOCK_GATE_TREE_2] = { 3 * n, 0, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_1] = { 4 * n, 0, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_2] = { 5 * n, 0, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_3] = { 6 * n, 0, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_4] = { 7 * n, 0, n, n };	
	dict_rect[(int)Tile::BLOCK_TREE_5] = { 8 * n, 0, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_6] = { 9 * n, 0, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_7] = { 10 * n, 0, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_8] = { 11 * n, 0, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_9] = { 12 * n, 0, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_10] = { 13 * n, 0, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_11] = { 14 * n, 0, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_12] = { 15 * n, 0, n, n };

	dict_rect[(int)Tile::BLOCK_TREE_13] = { 0,  n, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_14] = { n,  n, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_15] = { 2 * n, n, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_16] = { 3 * n, n, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_17] = { 4 * n, n, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_18] = { 5 * n, n, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_19] = { 6 * n, n, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_20] = { 7 * n, n, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_21] = { 8 * n, n, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_22] = { 9 * n, n, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_23] = { 10 * n, n, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_24] = { 11 * n, n, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_25] = { 12 * n, n, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_26] = { 13 * n, n, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_27] = { 14 * n, n, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_28] = { 15 * n, n, n, n };

	dict_rect[(int)Tile::BLOCK_TREE_29] = { 0,  n * 2, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_30] = { n,  n * 2, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_31] = { 2 * n, n * 2, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_32] = { 3 * n, n * 2, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_WALL] = { 4 * n, n * 2, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_1] = { 5 * n, n * 2, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_2] = { 6 * n, n * 2, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_3] = { 7 * n, n * 2, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_4] = { 8 * n, n * 2, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_5] = { 9 * n, n * 2, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_6] = { 10 * n, n * 2, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_7] = { 11 * n, n * 2, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_WINDOW_1] = { 12 * n, n * 2, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_WINDOW_2] = { 13 * n, n * 2, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_WINDOW_3] = { 14 * n, n * 2, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_WINDOW_4] = { 15 * n, n * 2, n, n };

	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_WINDOW_5] = { 0,  n * 3, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_WINDOW_6] = { n,  n * 3, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_WINDOW_7] = { 2 * n, n * 3, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_WINDOW_8] = { 3 * n, n * 3, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_WINDOW_9] = { 4 * n, n * 3, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_WINDOW_10] = { 5 * n, n * 3, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_WINDOW_11] = { 6 * n, n * 3, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_WINDOW_12] = { 7 * n, n * 3, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_INSIDE_1] = { 8 * n, n * 3, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_INSIDE_2] = { 9 * n, n * 3, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_INSIDE_WINDOW_1] = { 10 * n, n * 3, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_INSIDE_WINDOW_2] = { 11 * n, n * 3, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_INSIDE_3] = { 12 * n, n * 3, n, n };
	dict_rect[(int)Tile::BLOCK_COLUMN_STATUE_1] = { 13 * n, n * 3, n, n };
	dict_rect[(int)Tile::BLOCK_COLUMN_STATUE_2] = { 14 * n, n * 3, n, n };
	dict_rect[(int)Tile::BLOCK_COLUMN_STATUE_3] = { 15 * n, n * 3, n, n };

	dict_rect[(int)Tile::BLOCK_COLUMN_STATUE_4] = { 0,  n * 4, n, n };
	dict_rect[(int)Tile::BLOCK_COLUMN_STATUE_5] = { n,  n * 4, n, n };
	dict_rect[(int)Tile::BLOCK_COLUMN_STATUE_6] = { 2 * n, n * 4, n, n };
	dict_rect[(int)Tile::BLOCK_COLUMN_STATUE_7] = { 3 * n, n * 4, n, n };
	dict_rect[(int)Tile::BLOCK_PLATFORM_1] = { 4 * n, n * 4, n, n };
	dict_rect[(int)Tile::BLOCK_PLATFORM_2] = { 5 * n, n * 4, n, n };
	dict_rect[(int)Tile::BLOCK_PLATFORM_3] = { 6 * n, n * 4, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_WALL] = { 7 * n, n * 4, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_WALL_DIRT_1] = { 8 * n, n * 4, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_WALL_DIRT_2] = { 9 * n, n * 4, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_WALL_DIRT_3] = { 10 * n, n * 4, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_WALL_DIRT_4] = { 11 * n, n * 4, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_WALL_DIRT_5] = { 12 * n, n * 4, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_WALL_DIRT_6] = { 13 * n, n * 4, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_WALL_DIRT_7] = { 14 * n, n * 4, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_WALL_DIRT_8] = { 15 * n, n * 4, n, n };

	dict_rect[(int)Tile::BLOCK_WHITE_WALL_DIRT_9] = { 0,  n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_WALL_DIRT_10] = { n,  n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_STAIRS_RIGHT_1] = { 2 * n, n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_WALL_DIRT_11] = { 3 * n, n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_STAIRS_LEFT_1] = { 4 * n, n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_WALL_DIRT_12] = { 5 * n, n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_ARCH_1] = { 6 * n, n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_ARCH_2] = { 7 * n, n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_ARCH_3] = { 8 * n, n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_ARCH_4] = { 9 * n, n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_ARCH_5] = { 10 * n, n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_ARCH_6] = { 11 * n, n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_WINDOW] = { 12 * n, n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_COLUMN_1] = { 13 * n, n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_COLUMN_2] = { 14 * n, n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_COLUMN_3] = { 15 * n, n * 5, n, n };

	dict_rect[(int)Tile::BLOCK_COLUMN_4] = { 0,  n * 6, n, n };
	dict_rect[(int)Tile::BLOCK_COLUMN_5] = { n,  n * 6, n, n };
	dict_rect[(int)Tile::BLOCK_COLUMN_6] = { 2 * n, n * 6, n, n };
	dict_rect[(int)Tile::BLOCK_COLUMN_7] = { 3 * n, n * 6, n, n };
	dict_rect[(int)Tile::BLOCK_COLUMN_8] = { 4 * n, n * 6, n, n };
	dict_rect[(int)Tile::BLOCK_CURTAINS_1] = { 5 * n, n * 6, n, n };
	dict_rect[(int)Tile::BLOCK_CURTAINS_2] = { 6 * n, n * 6, n, n };
	dict_rect[(int)Tile::BLOCK_CURTAINS_3] = { 7 * n, n * 6, n, n };
	dict_rect[(int)Tile::BLOCK_CURTAINS_4] = { 8 * n, n * 6, n, n };
	dict_rect[(int)Tile::BLOCK_CURTAINS_5] = { 9 * n, n * 6, n, n };
	dict_rect[(int)Tile::BLOCK_CURTAINS_6] = { 10 * n, n * 6, n, n };
	dict_rect[(int)Tile::BLOCK_CURTAINS_7] = { 11 * n, n * 6, n, n };
	dict_rect[(int)Tile::BLOCK_CURTAINS_8] = { 12 * n, n * 6, n, n };
	dict_rect[(int)Tile::BLOCK_CURTAINS_9] = { 13 * n, n * 6, n, n };
	dict_rect[(int)Tile::BLOCK_CURTAINS_10] = { 14 * n, n * 6, n, n };
	dict_rect[(int)Tile::BLOCK_CURTAINS_11] = { 15 * n, n * 6, n, n };

	dict_rect[(int)Tile::BLOCK_BLACK] = { 0,  n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_STONE_1] = { n,  n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_STONE_2] = { 2 * n, n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_STONE_3] = { 3 * n, n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_STONE_4] = { 4 * n, n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_STONE_5] = { 5 * n, n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_STAIRS_RIGHT_2] = { 6 * n, n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_STAIRS_LEFT_2] = { 7 * n, n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_DARK_GATE_1] = { 8 * n, n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_DARK_GATE_2] = { 9 * n, n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_DARK_GATE_3] = { 10 * n, n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_COLUMN_DIRT_1] = { 11 * n, n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_COLUMN_DIRT_2] = { 12 * n, n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_STONE_WINDOW] = { 13 * n, n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_STONE_6] = { 14 * n, n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_STONE_7] = { 15 * n, n * 7, n, n };

	dict_rect[(int)Tile::BLOCK_STONE_8] = { 0,  n * 8, n, n };
	dict_rect[(int)Tile::BLOCK_MOSS_1] = { n,  n * 8, n, n };
	dict_rect[(int)Tile::BLOCK_MOSS_2] = { 2 * n, n * 8, n, n };
	dict_rect[(int)Tile::BLOCK_MOSS_3] = { 3 * n, n * 8, n, n };
	dict_rect[(int)Tile::BLOCK_WATER_1] = { 4 * n, n * 8, n, n };
	dict_rect[(int)Tile::BLOCK_WATER_2] = { 5 * n, n * 8, n, n };
	dict_rect[(int)Tile::BLOCK_WATER_3] = { 6 * n, n * 8, n, n };
	dict_rect[(int)Tile::BLOCK_WATER_4] = { 7 * n, n * 8, n, n };
	dict_rect[(int)Tile::BLOCK_DOOR] = { 8 * n, n * 8, n, n };
	dict_rect[(int)Tile::BLOCK_GATE_2] = { 9 * n, n * 8, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_33] = { 10 * n, n * 8, n, n };
	dict_rect[(int)Tile::BLOCK_TREE_34] = { 11 * n, n * 8, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_8] = { 12 * n, n * 8, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_9] = { 13 * n, n * 8, n, n };
	dict_rect[(int)Tile::BLOCK_WALL_OUTSIDE_10] = { 14 * n, n * 8, n, n };
	dict_rect[(int)Tile::BLOCK_MOSS_4] = { 15 * n, n * 8, n, n };

	dict_rect[(int)Tile::BLOCK_STAIRS_LEFT_3] = { 0 * n, n * 9, n, n };
	dict_rect[(int)Tile::BLOCK_WHITE_WALL_2] = { 1 * n, n * 9, n, n };

	dict_rect[(int)Tile::BLOCK_STAIRS_LEFT_1_TOP] = { 4 * n, n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_STAIRS_RIGHT_1_TOP] = { 2 * n, n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_STAIRS_LEFT_2_TOP] = { 7 * n, n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_STAIRS_RIGHT_2_TOP] = { 6 * n, n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_STAIRS_LEFT_3_TOP] = { 0 * n, n * 9, n, n };

	dict_rect[(int)Tile::BLOCK_STAIRS_LEFT_1_BOT] = { 4 * n, n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_STAIRS_RIGHT_1_BOT] = { 2 * n, n * 5, n, n };
	dict_rect[(int)Tile::BLOCK_STAIRS_LEFT_2_BOT] = { 7 * n, n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_STAIRS_RIGHT_2_BOT] = { 6 * n, n * 7, n, n };
	dict_rect[(int)Tile::BLOCK_STAIRS_LEFT_3_BOT] = { 0 * n, n * 9, n, n };
	

}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES, "img/tiles.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);


	if (data.LoadTexture(Resource::IMG_ITEMS, "img/items.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_items = data.GetTexture(Resource::IMG_ITEMS);


	fire = new Sprite(img_items);
	if (fire == nullptr)
	{
		LOG("Failed to allocate memory for fire sprite");
		return AppStatus::ERROR;
	}
	fire->SetNumberAnimations(1);
	fire->SetAnimationDelay(0, ANIM_DELAY_FIRE);
	fire->AddKeyFrame(0, dict_rect[(int)Tile::FIRE_FRAME0]);
	fire->AddKeyFrame(0, dict_rect[(int)Tile::FIRE_FRAME1]);
	fire->SetAnimation(0);


	laser = new Sprite(img_tiles);
	if (laser == nullptr)
	{
		LOG("Failed to allocate memory for laser sprite");
		return AppStatus::ERROR;
	}
	laser->SetNumberAnimations(1);
	laser->SetAnimationDelay(0, ANIM_DELAY_FIRE);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME0]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME1]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME2]);
	laser->SetAnimation(0);

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w*h;
	width = w;
	height = h;

	if (map != nullptr)	delete[] map;

	map = new Tile[size];
	if (map == nullptr)	
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(map, data, size * sizeof(int));

	return AppStatus::OK;
}
void TileMap::ClearObjectEntityPositions()
{
	int i;
	Tile tile;
	for (i = 0; i < size; ++i)
	{
		tile = map[i];
		if (IsTileEntity(tile) || IsTileObject(tile) || tile == Tile::EMPTY)
			map[i] = Tile::AIR;
	}
}
void TileMap::Update()
{
	fire->Update();
	laser->Update();
}
Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y*width;
	if(idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
		return Tile::AIR;
	}
	return map[x + y * width];
}
bool TileMap::IsTileObject(Tile tile) const
{
	return Tile::OBJECT_FIRST <= tile && tile <= Tile::OBJECT_LAST;
}
bool TileMap::IsTileEntity(Tile tile) const
{
	return Tile::ENTITY_FIRST <= tile && tile <= Tile::ENTITY_LAST;
}
bool TileMap::IsTileSolid(Tile tile) const
{	
	//return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST);
	return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST || (tile == Tile::BLOCK_PLATFORM_1 || tile == Tile::BLOCK_PLATFORM_2 || tile == Tile::BLOCK_PLATFORM_3));
}
bool TileMap::IsTileLadderTopLeft(Tile tile) const
{
	return tile == Tile::BLOCK_STAIRS_LEFT_1_TOP || tile == Tile::BLOCK_STAIRS_LEFT_2_TOP || tile == Tile::BLOCK_STAIRS_LEFT_3_TOP;
}
bool TileMap::IsTileLadderTopRight(Tile tile) const
{
	return tile == Tile::BLOCK_STAIRS_RIGHT_1_TOP || tile == Tile::BLOCK_STAIRS_RIGHT_2_TOP;
}
bool TileMap::IsTileLadderTop(Tile tile) const
{
	return tile == Tile::BLOCK_STAIRS_LEFT_1_TOP || tile == Tile::BLOCK_STAIRS_LEFT_2_TOP || tile == Tile::BLOCK_STAIRS_LEFT_3_TOP || tile == Tile::BLOCK_STAIRS_RIGHT_1_TOP || tile == Tile::BLOCK_STAIRS_RIGHT_2_TOP;
}
bool TileMap::IsTileLadder(Tile tile) const
{
	return tile == Tile::BLOCK_STAIRS_RIGHT_1_BOT || tile == Tile::BLOCK_STAIRS_RIGHT_2_BOT || tile == Tile::BLOCK_STAIRS_LEFT_1_BOT || 
		tile == Tile::BLOCK_STAIRS_LEFT_2_BOT || tile == Tile::BLOCK_STAIRS_LEFT_3_BOT || tile == Tile::BLOCK_STAIRS_LEFT_1 || 
		tile == Tile::BLOCK_STAIRS_RIGHT_1 || tile == Tile::BLOCK_STAIRS_LEFT_2 || tile == Tile::BLOCK_STAIRS_RIGHT_2 || 
		tile == Tile::BLOCK_STAIRS_LEFT_3 || tile == Tile::BLOCK_STAIRS_RIGHT_1_TOP || tile == Tile::BLOCK_STAIRS_LEFT_1_TOP;
}
bool TileMap::IsTileLadderBotLeft(Tile tile) const
{
	return tile == Tile::BLOCK_STAIRS_LEFT_1_BOT || tile == Tile::BLOCK_STAIRS_LEFT_2_BOT || tile == Tile::BLOCK_STAIRS_LEFT_3_BOT;
}
bool TileMap::IsTileLadderBotRight(Tile tile) const
{
	return tile == Tile::BLOCK_STAIRS_RIGHT_1_BOT || tile == Tile::BLOCK_STAIRS_RIGHT_2_BOT;
}
bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionGround(const AABB& box, int *py) const
{
	Point p(box.pos.x, *py);	//control point
	int tile_y;

	if (CollisionY(p, box.width))
	{
		tile_y = p.y / TILE_SIZE;

		*py = tile_y * TILE_SIZE - 1;
		return true;
	}
	return false;
}
bool TileMap::TestFalling(const AABB& box) const
{
	return !CollisionY(box.pos + Point(0, box.height), box.width);
}
bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;
	
	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileSolid(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile) || IsTileLadderTopRight(tile) || IsTileLadderTopLeft(tile))
			return true;
	}
	return false;
}
bool TileMap::TestOnLadder(const AABB& box, int* px) const
{
	int right, left, bottom2, bottom1;
	int tx1, tx2, ty1, ty2;
	Tile tile1, tile2;

	//Control points
	left = box.pos.x + box.width - 8;
	right = box.pos.x + box.width;
	bottom2 = box.pos.y + box.height - 8;
	bottom1 = box.pos.y + box.height;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty2 = bottom2 / TILE_SIZE;
	ty1 = bottom1 / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder

	tile2 = GetTileIndex(tx2, ty2);
	tile1 = GetTileIndex(tx1, ty1);
	if (IsTileLadder(tile2))
	{
		*px = GetLadderCenterPos(right, bottom2) - (box.width / 2) + 1;
		return true;
	}
	else if (IsTileLadder(tile1))
	{
		*px = GetLadderCenterPos(left, bottom1) - (box.width / 2) + 1;
		return true;
	}
	return false;
}
bool TileMap::TestOnLadderLeft(const AABB& box, int* px) const
{
	int right, left, bottom2, bottom1;
	int tx1, tx2, ty1, ty2;
	Tile tile1, tile2;

	// Control points
	left = box.pos.x - box.width + 8;                 // Use the left side of the box
	right = box.pos.x;            // A point a bit to the right of the left side
	bottom2 = box.pos.y + box.height - 8;
	bottom1 = box.pos.y + box.height;

	// Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty2 = bottom2 / TILE_SIZE;
	ty1 = bottom1 / TILE_SIZE;

	// To be able to climb up or down, both control points must be on ladder

	tile2 = GetTileIndex(tx2, ty2);
	tile1 = GetTileIndex(tx1, ty1);
	if (IsTileLadder(tile2))
	{
		*px = GetLadderCenterPos(right, bottom2) - (box.width / 2) + 1;
		return true;
	}
	else if (IsTileLadder(tile1))
	{
		*px = GetLadderCenterPos(left, bottom1) - (box.width / 2) + 1;
		return true;
	}
	return false;
}
bool TileMap::TestOnLadderTopLeft(const AABB& box, int* px) const
{
	int right, left, bottom2, bottom1;
	int tx1, tx2, ty1, ty2;
	Tile tile1, tile2;

	//Control points
	left = box.pos.x - box.width + 8;                 // Use the left side of the box
	right = box.pos.x;            // A point a bit to the right of the left side
	bottom2 = box.pos.y + box.height - 8;
	bottom1 = box.pos.y + box.height;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty2 = bottom2 / TILE_SIZE;
	ty1 = bottom1 / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile2 = GetTileIndex(tx2, ty2);
	tile1 = GetTileIndex(tx1, ty1);
	if (IsTileLadderTopLeft(tile2))
	{
		*px = GetLadderCenterPos(right, bottom2) + (box.width / 2) + 1;
		return true;
	}
	//else if (IsTileLadder(tile1))
	//{
	//	*px = GetLadderCenterPos(left, bottom1) + (box.width / 2) + 1;
	//	return true;
	//}
	else
		return false;
}
bool TileMap::TestOnLadderTop(const AABB& box, int* px) const
{
	int right, left, bottom2, bottom1;
	int tx1, tx2, ty1, ty2;
	Tile tile1, tile2;

	//Control points
	left = box.pos.x + box.width - 8;
	right = box.pos.x + box.width;
	bottom2 = box.pos.y + box.height - 8;
	bottom1 = box.pos.y + box.height;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty2 = bottom2 / TILE_SIZE;
	ty1 = bottom1 / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile2 = GetTileIndex(tx2, ty2);
	tile1 = GetTileIndex(tx1, ty1);
	if (IsTileLadderTop(tile2))
	{
		*px = GetLadderCenterPos(right, bottom2) - (box.width / 2) + 1;
		return true;
	}
	else if (IsTileLadderTop(tile1))
	{
		*px = GetLadderCenterPos(left, bottom1) - (box.width / 2) + 1;
		return true;
	}
	return false;
}
bool TileMap::TestOnLadderTopRight(const AABB& box, int* px) const
{
	int right, left, bottom2, bottom1;
	int tx1, tx2, ty1, ty2;
	Tile tile1, tile2;

	//Control points
	left = box.pos.x + box.width - 8;
	right = box.pos.x + box.width;
	bottom2 = box.pos.y + box.height - 8;
	bottom1 = box.pos.y + box.height;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty2 = bottom2 / TILE_SIZE;
	ty1 = bottom1 / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile2 = GetTileIndex(tx2, ty2);
	tile1 = GetTileIndex(tx1, ty1);
	if (IsTileLadderTopRight(tile2))
	{
		*px = GetLadderCenterPos(right, bottom2) - (box.width / 2) + 1;
		return true;
	}
	else if (IsTileLadderTopRight(tile1))
	{
		*px = GetLadderCenterPos(left, bottom1) - (box.width / 2) + 1;
		return true;
	}
	return false;
}
bool TileMap::TestOnLadderBotRight(const AABB& box, int* px) const
{
	int right, left, bottom2, bottom1;
	int tx1, tx2, ty1, ty2;
	Tile tile1, tile2;

	//Control points
	left = box.pos.x + box.width - 8;
	right = box.pos.x + box.width;
	bottom2 = box.pos.y + box.height - 8;
	bottom1 = box.pos.y + box.height;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty2 = bottom2 / TILE_SIZE;
	ty1 = bottom1 / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile2 = GetTileIndex(tx2, ty2);
	tile1 = GetTileIndex(tx1, ty1);
	if (IsTileLadderBotRight(tile2))
	{
		*px = GetLadderCenterPos(right, bottom2) - (box.width / 2) + 1;
		return true;
	}
	else
		return false;
}
bool TileMap::TestOnLadderBotLeft(const AABB& box, int* px) const
{
	int right, left, bottom;
	int tx1, tx2, ty1, ty2;
	Tile tile1, tile2;

	//Control points
	left = box.pos.x + box.width - 8;
	right = box.pos.x + box.width;
	bottom = box.pos.y + box.height;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty2 = bottom / TILE_SIZE;
	ty1 = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile2 = GetTileIndex(tx2, ty2);
	tile1 = GetTileIndex(tx1, ty1);
	if (IsTileLadderBotLeft(tile2))
	{
		*px = GetLadderCenterPos(right, bottom) - (box.width / 2) + 1;
		return true;
	}
	else
		return false;
}
int TileMap::GetLadderCenterPos(int pixel_x, int pixel_y) const //dis is gut
{
	int tx, ty;

	tx = pixel_x / TILE_SIZE;
	ty = pixel_y / TILE_SIZE;
	Tile tile = GetTileIndex(tx, ty);

	if (tile == Tile::BLOCK_STAIRS_RIGHT_1_BOT || tile == Tile::BLOCK_STAIRS_RIGHT_2_BOT 
		|| tile == Tile::BLOCK_STAIRS_RIGHT_1  || tile == Tile::BLOCK_STAIRS_RIGHT_2)		
		return tx * TILE_SIZE;
	else if (tile == Tile::BLOCK_STAIRS_LEFT_1_BOT || tile == Tile::BLOCK_STAIRS_LEFT_2_BOT 
		|| tile == Tile::BLOCK_STAIRS_LEFT_3_BOT || tile == Tile::BLOCK_STAIRS_LEFT_1 ||
		 tile == Tile::BLOCK_STAIRS_LEFT_2 || tile == Tile::BLOCK_STAIRS_LEFT_3)		
		return tx * TILE_SIZE + TILE_SIZE;
	else if (tile == Tile::BLOCK_STAIRS_LEFT_1_TOP || tile == Tile::BLOCK_STAIRS_LEFT_2_TOP || tile == Tile::BLOCK_STAIRS_LEFT_3_TOP)
		return tx * TILE_SIZE;
	else if (tile == Tile::BLOCK_STAIRS_RIGHT_1_TOP || tile == Tile::BLOCK_STAIRS_RIGHT_2_TOP)
		return tx * TILE_SIZE + TILE_SIZE;
	else
	{
		LOG("Internal error, tile should be a LADDER, coord: (%d,%d), tile type: %d", pixel_x, pixel_y, (int)tile);
		return 0;
	}
}
AABB TileMap::GetSweptAreaX(const AABB& hitbox) const
{
	AABB box;
	int column, x, y, y0, y1;
	bool collision;

	box.pos.y = hitbox.pos.y;
	box.height = hitbox.height;

	column = hitbox.pos.x / TILE_SIZE;
	y0 = hitbox.pos.y / TILE_SIZE;
	y1 = (hitbox.pos.y + hitbox.height - 1) / TILE_SIZE;

	//Compute left tile index
	collision = false;
	x = column - 1;
	while (!collision && x >= 0)
	{
		//Iterate over the tiles within the vertical range
		for (y = y0; y <= y1; ++y)
		{
			//One solid tile is sufficient
			if (IsTileSolid(GetTileIndex(x, y)))
			{
				collision = true;
				break;
			}
		}
		if (!collision) x--;
	}
	box.pos.x = (x + 1) * TILE_SIZE;

	//Compute right tile index
	collision = false;
	x = column + 1;
	while (!collision && x < LEVEL_WIDTH)
	{
		//Iterate over the tiles within the vertical range
		for (y = y0; y <= y1; ++y)
		{
			//One solid tile is sufficient
			if (IsTileSolid(GetTileIndex(x, y)))
			{
				collision = true;
				break;
			}
		}
		if (!collision) x++;
	}
	box.width = x * TILE_SIZE - box.pos.x;

	return box;
}
void TileMap::Render()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = map[i * width + j];
			if (tile != Tile::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

				if ((int)tile == 800) {
					bool is
						= true;
				}
				
				if (tile != Tile::FIRE)
				{
					rc = dict_rect[(int)tile];
					//DrawTextureRec(*img_items, rc, pos, WHITE);
					DrawTextureRec(*img_tiles, rc, pos, WHITE);
				}
				else if(tile != Tile::FIRE)
				{
					rc = dict_rect[(int)tile];
					//DrawTextureRec(*img_items, rc, pos, WHITE);
					DrawTextureRec(*img_tiles, rc, pos, WHITE);
				}
				else
				{
					fire->Draw((int)pos.x, (int)pos.y);
				}
			}
		}
	}
}
void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance(); 
	data.ReleaseTexture(Resource::IMG_TILES);
	data.ReleaseTexture(Resource::IMG_ITEMS);

	laser->Release();
	fire->Release();

	dict_rect.clear();
}