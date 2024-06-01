#pragma once
#include "Entity.h"
#include "TileMap.h"
//#include "EnemyManager.h"

//Representation model size: 32x32
#define SLIME_FRAME_SIZE		32
//Logical model size: 24x30
#define SLIME_PHYSICAL_WIDTH	24
#define SLIME_PHYSICAL_HEIGHT	30

//Representation model size: 16x32
#define ZOMBIE_FRAME_SIZE_WIDTH		16
#define ZOMBIE_FRAME_SIZE_HEIGHT	32
//Logical model size: 12x28
#define ZOMBIE_PHYSICAL_WIDTH	12
#define ZOMBIE_PHYSICAL_HEIGHT	28

// TODO Fix Leopard's hitbox
//Representation model size: 32x16
#define BLACKLEOPARD_FRAME_SIZE_WIDTH		32
#define BLACKLEOPARD_FRAME_SIZE_HEIGHT	16
//Logical model size: 28x12
#define BLACKLEOPARD_PHYSICAL_WIDTH	28
#define BLACKLEOPARD_PHYSICAL_HEIGHT	12

//Representation model size: 16x16
#define BAT_FRAME_SIZE_WIDTH	16
#define BAT_FRAME_SIZE_HEIGHT	16
//Logical model size: 12x12
#define BAT_PHYSICAL_WIDTH	12
#define BAT_PHYSICAL_HEIGHT	12

//Representation model size: 16x32
#define FISHMAN_FRAME_SIZE_WIDTH		16
#define FISHMAN_FRAME_SIZE_HEIGHT	32
//Logical model size: 12x28
#define FISHMAN_PHYSICAL_WIDTH	12
#define FISHMAN_PHYSICAL_HEIGHT	28

//Representation model size: 32x32
#define TURRET_FRAME_SIZE		32
//Logical model size: 0x0
#define TURRET_PHYSICAL_WIDTH	32
#define TURRET_PHYSICAL_HEIGHT	26

#define CANDLE_FRAME_SIZE 16
#define CANDLE_PHYSICAL_SIZE 16

#define PYRE_FRAME_SIZE 16
#define PYRE_PHYSICAL_SIZE 16

#define BLOCKS_FRAME_SIZE 32



enum class EnemyType { ZOMBIE, SLIME, TURRET, BAT, FISHMAN, BLACKLEOPARD, PYRE, CANDLE, BLOCKS };

class Enemy : public Entity //, public EnemyManager
{
public:
	Enemy(const Point& p, int width, int height, int frame_width, int frame_height);
	virtual ~Enemy();

	//Draw the maximum visibility area of the enemy
	void DrawVisibilityArea(const Color& col) const;

	//Pure virtual functions, any class inheriting from this class must provide its own implementations

	//Initialize the enemy with the specified look and area
	virtual AppStatus Initialise(Look look, const AABB& area) = 0;

	//Update the enemy according to its logic, return true if the enemy must shoot
	virtual bool Update(const AABB& box) = 0;

	//Retrieve the position and direction of the shot to be thrown
	virtual void GetShootingPosDir(Point* pos, Point* dir) const = 0;
	EnemyType type;
	TileMap* map;
	bool inArea;

protected:
	//Return true if the given hitbox is within the visibility area and the enemy is facing it
	bool IsVisible(const AABB& hitbox);

	
	Look look;
	AABB visibility_area;
	
};

