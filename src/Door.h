#pragma once
#include "Enemy.h"

#define DOOR_SPEED			1
#define DOOR_ANIM_DELAY	(ANIM_DELAY)


enum class DoorState { STATIC, OPENING };
enum class DoorAnim { STATIC, OPENING, NUM_ANIMATIONS };

struct DoorStep
{
	Point speed;	//direction
	int frames;		//duration in number of frames
	int anim;		//graphical representation
};


class Door : public Enemy
{
public:
	Door(const Point& p, int width, int height, int frame_width, int frame_height);
	~Door();

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

	//Update looking direction according to the current step of the pattern
	void UpdateLook(int anim_id);

	int attack_delay;	//delay between attacks
	DoorState state;

	int current_step;	//current step of the pattern
	int current_frames;	//number of frames in the current step
	int doorFrames;
	std::vector<DoorStep> pattern;
};