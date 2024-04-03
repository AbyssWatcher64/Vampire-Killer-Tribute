#pragma once
#include "Entity.h"
#include "TileMap.h"

// I tried to use Look isntead of ZLook but I had some issues
// since it's already declared in Player.h
// we should consider fixing this
enum class ZLook { ZRIGHT, ZLEFT };


// Also I believe the zombie is properly prepared, we just need to get it to
// spawn in some way.
enum class ZombieAnim 
{
	MOVING_LEFT, MOVING_RIGHT,

	NUM_ANIMATIONS
};

class Zombie : public Entity
{
public:
	Zombie(const Point& p, int width, int height, ZLook view);
	~Zombie();
	
	AppStatus Initialise();
	void Update();

	void SetLook(ZLook view);
	bool IsLookingRight() const;

	void WalkingLeft();
	void WalkingRight();

	void DrawDebug(const Color& col) const;

	void Release();

private:
	void SetAnimation(int id);
	ZLook look;

};

