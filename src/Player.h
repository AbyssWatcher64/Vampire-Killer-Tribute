#pragma once
#include "Entity.h"
#include "TileMap.h"

//Logic states
enum class State { IDLE, WALKING, JUMPING, FALLING, CLIMBING, DEAD };
enum class Look { RIGHT, LEFT };

//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT,
	WALKING_LEFT, WALKING_RIGHT,
	JUMPING_LEFT, JUMPING_RIGHT,
	FALLING_LEFT, FALLING_RIGHT,
	CLIMBING_LEFT, CLIMBING_RIGHT,
	SHOCK_LEFT, SHOCK_RIGHT,
	TELEPORT_LEFT, TELEPORT_RIGHT,
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

	void DrawDebug(const Color& col) const;

	void Release();

private:
	void SetAnimation(int id);

	State state;
	Look look;
};

