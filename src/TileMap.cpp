#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>

TileMap::TileMap()
{
	map = nullptr;
	width = 0;
	height = 0;
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
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;
	//TODO: Add the rest of the blocks
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

}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES, "img/tiles.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);

	laser = new Sprite(img_tiles);
	if (laser == nullptr)
	{
		LOG("Failed to allocate memory for laser sprite");
		return AppStatus::ERROR;
	}
	laser->SetNumberAnimations(1);
	laser->SetAnimationDelay(0, ANIM_DELAY);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME0]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME1]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME2]);
	laser->SetAnimation(0);

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	int size = w*h;

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
void TileMap::Update()
{
	laser->Update();
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

				if (tile != Tile::LASER)
				{
					rc = dict_rect[(int)tile];
					DrawTextureRec(*img_tiles, rc, pos, WHITE);
				}
				else
				{
					laser->Draw((int)pos.x, (int)pos.y);
				}
			}
		}
	}
}
void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance(); 
	data.ReleaseTexture(Resource::IMG_TILES);

	laser->Release();

	dict_rect.clear();
}