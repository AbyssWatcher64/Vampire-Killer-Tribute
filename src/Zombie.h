#pragma once
#include "Enemy.h"
#include "TileMap.h"




#define ZOMBIE_SPEED			1
#define ZOMBIE_ANIM_DELAY	(4*ANIM_DELAY)

#define ZOMBIE_SHOT_OFFSET_X_LEFT	-14
#define ZOMBIE_SHOT_OFFSET_X_RIGHT	 26
#define ZOMBIE_SHOT_OFFSET_Y			-22
		
#define ZOMBIE_SHOT_SPEED	4

enum class ZombieState { ROAMING, ATTACK };
enum class ZombieAnim {	WALKING_LEFT, WALKING_RIGHT, NUM_ANIMATIONS };

struct Step
{
	Point speed;	//direction
	int frames;		//duration in number of frames
	int anim;		//graphical representation
};

class Zombie : public Enemy
{
public:
	Zombie(const Point& p, int width, int height, int frame_width, int frame_height);
	~Zombie();

	//Initialize the enemy with the specified look and area
	AppStatus Initialise(Look look, const AABB& area) override;
	//void SetTileMap(TileMap* tilemap);

	//Update the enemy according to its logic, return true if the enemy must shoot
	bool Update(const AABB& box) override;

	//Retrieve the position and direction of the shot to be thrown
	void GetShootingPosDir(Point* pos, Point* dir) const override;

	void SetTileMap(TileMap* tilemap);

private:
	//Create the pattern behaviour
	void InitPattern();
	void MoveX();

	//Update looking direction according to the current step of the pattern
	void UpdateLook(int anim_id);

	TileMap* map;

	int attack_delay;	//delay between attacks
	ZombieState state;

	int current_step;	//current step of the pattern
	int current_frames;	//number of frames in the current step
	std::vector<Step> pattern;
};




//#pragma once
//#include "Entity.h"
//#include "TileMap.h"
//
////Representation model size: 16x32
////#define PLAYER_FRAME_SIZE		16
//#define ENEMY_FRAME_SIZE_WIDTH	16
//#define ENEMY_FRAME_SIZE_HEIGHT	32
//
////Logical model size: 12x28
//#define ENEMY_PHYSICAL_WIDTH	12
//#define ENEMY_PHYSICAL_HEIGHT	28
//
////Horizontal speed and vertical speed while falling down
//#define ENEMY_SPEED			2
//
////Vertical speed while on a ladder
//#define ENEMY_LADDER_SPEED		1
//
////Frame animation delay while on a ladder
//#define ENEMY_LADDER_DELAY		(2*ANIM_DELAY)
//
//#define ENEMY_LEVITATING_SPEED	4
//
////Gravity affects jumping velocity when jump_delay is 0
//#define GRAVITY_FORCE			1
//
////Logic states
//enum class EnemyState { IDLE, WALKING  };
//enum class EnemyLook { RIGHT, LEFT };
//
////Rendering states
//enum class EnemyAnim {
//	
//	IDLE_LEFT, IDLE_RIGHT,
//
//	WALKING_LEFT, WALKING_RIGHT,
//
//	NUM_ANIMATIONS
//};
//
//class Enemy: public Entity
//{
//public:
//	Enemy(const Point& p, EnemyState s, EnemyLook view);
//	~Enemy();
//	
//	AppStatus Initialise();
//	void SetTileMap(TileMap* tilemap);
//
//	int GetXPos();
//	int GetYPos();
//
//	void Update();
//	void DrawDebug(const Color& col) const;
//	void Release();
//
//
//
//private:
//	bool IsLookingRight() const;
//	bool IsLookingLeft() const;
//
//	//Enemy mechanics
//	void MoveX();
//	void MoveY();
//
//	//Animation management
//	void SetAnimation(int id);
//	EnemyAnim GetAnimation();
//	void Stop();
//	void StartWalkingLeft();
//	void StartWalkingRight();
//	void ChangeAnimRight();
//	void ChangeAnimLeft();
//
//	EnemyState state;
//	EnemyLook look;
//
//	TileMap *map;
//
//};
//
