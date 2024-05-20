#pragma once
#include "Enemy.h"

#define FISHMAN_INITIAL_SPEED	16
#define FISHMAN_SPEED			1
#define FISHMAN_ANIM_DELAY	(4*ANIM_DELAY)

#define FISHMAN_SHOT_OFFSET_X_LEFT	-14
#define FISHMAN_SHOT_OFFSET_X_RIGHT	 26
#define FISHMAN_SHOT_OFFSET_Y			-22
		
#define FISHMAN_JUMP_FORCE	20

#define FISHMAN_SHOT_SPEED	4
#define GRAVITY_FORCE	1

enum class FishmanState { ROAMING };
enum class FishmanAnim {	WALKING_LEFT, WALKING_RIGHT, NUM_ANIMATIONS };

struct FishmanStep
{
	Point speed;	//direction
	int frames;		//duration in number of frames
	int anim;		//graphical representation
};

class Fishman : public Enemy
{
public:
	Fishman(const Point& p, int width, int height, int frame_width, int frame_height);
	~Fishman();

	//Initialize the enemy with the specified look and area
	AppStatus Initialise(Look look, const AABB& area) override;
	//void SetTileMap(TileMap* tilemap);

	//Update the enemy according to its logic, return true if the enemy must shoot
	bool Update(const AABB& box) override;

	//Retrieve the position and direction of the shot to be thrown
	void GetShootingPosDir(Point* pos, Point* dir) const override;
private:
	//Create the pattern behaviour
	void InitPattern();
	void MoveX();
	void MoveY();
	bool canWalk;
	bool hasSpawned;
	//Update looking direction according to the current step of the pattern
	void UpdateLook(int anim_id);

	void FishManInitialFalling();

	int attack_delay;	//delay between attacks
	FishmanState state;

	int current_step;	//current step of the pattern
	int current_frames;	//number of frames in the current step
	std::vector<FishmanStep> pattern;
};

