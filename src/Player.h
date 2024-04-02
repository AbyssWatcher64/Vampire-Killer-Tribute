#pragma once
#include "Entity.h"
#include "TileMap.h"

//Logic states
enum class State { IDLE, WALKING, ATTACKING, JUMPING, FALLING, CLIMBING, DEAD };
enum class Look { RIGHT, LEFT };

//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT,
	IDLE_LEFT_SHIELD, IDLE_RIGHT_SHIELD,

	WALKING_LEFT, WALKING_RIGHT,
	WALKING_LEFT_SHIELD, WALKING_RIGHT_SHIELD,

	CROUCHING_LEFT, CROUCHING_RIGHT,
	CROUCHING_LEFT_SHIELD, CROUCHING_RIGHT_SHIELD,

	WALKING_STAIRS_LEFT, WALKING_STAIRS_RIGHT,
	WALKING_STAIRS_LEFT_SHIELD, WALKING_STAIRS_RIGHT_SHIELD,

	JUMPING_LEFT, JUMPING_RIGHT,
	JUMPING_LEFT_SHIELD, JUMPING_RIGHT_SHIELD,

	WALKING_INTO,
	DYING,

	ATTACKING_LEFT_WHIP, ATTACKING_RIGHT_WHIP,
	ATTACKING_CROUCHING_LEFT_WHIP, ATTACKING_CROUCHING_RIGHT_WHIP,
	ATTACKING_UPSTAIRS_LEFT_WHIP, ATTACKING_UPSTAIRS_RIGHT_WHIP,
	ATTACKING_DOWNSTAIRS_LEFT_WHIP, ATTACKING_DOWNSTAIRS_RIGHT_WHIP,

	ATTACKING_LEFT_MORNING_STAR, ATTACKING_RIGHT_MORNING_STAR,
	ATTACKING_CROUCHING_LEFT_MORNING_STAR, ATTACKING_CROUCHING_RIGHT_MORNING_STAR,
	ATTACKING_UPSTAIRS_LEFT_MORNING_STAR, ATTACKING_UPSTAIRS_RIGHT_MORNING_STAR,
	ATTACKING_DOWNSTAIRS_LEFT_MORNING_STAR, ATTACKING_DOWNSTAIRS_RIGHT_MORNING_STAR,

	ATTACKING_LEFT_RANGED, ATTACKING_RIGHT_RANGED,
	ATTACKING_CROUCHING_LEFT_RANGED, ATTACKING_CROUCHING_RIGHT_RANGED,
	ATTACKING_UPSTAIRS_LEFT_RANGED, ATTACKING_UPSTAIRS_RIGHT_RANGED,
	ATTACKING_DOWNSTAIRS_LEFT_RANGED, ATTACKING_DOWNSTAIRS_RIGHT_RANGED,

	NUM_ANIMATIONS
};

class Player: public Entity
{
public:
	Player(const Point& p, int width, int height, State s, Look view);
	~Player();
	
	AppStatus Initialise();
	void Update();
	
	void SetState(State s);
	State GetState() const;

	void SetLook(Look view);
	bool IsLookingRight() const;

	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	void Attack();
	void Death();

	void DrawDebug(const Color& col) const;

	void Release();

private:
	void SetAnimation(int id);

	State state;
	Look look;
};

