#pragma once
#include "Entity.h"
#include "TileMap.h"
#include "SoundManager.h"

//Representation model size: 16x32
//#define PLAYER_FRAME_SIZE		16
#define PLAYER_FRAME_SIZE_WIDTH		16
#define PLAYER_FRAME_SIZE_HEIGHT	32

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	12
#define PLAYER_PHYSICAL_HEIGHT	28
#define PLAYER_PHYSICAL_HEIGHT_CROUCH	20

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED			2

//Vertical speed while on a ladder
#define PLAYER_LADDER_SPEED		1

//Frame animation delay while on a ladder
#define ANIM_LADDER_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
#define PLAYER_JUMP_FORCE		10

//Frame delay for updating the jump velocity
#define PLAYER_JUMP_DELAY		2

//Player is levitating when abs(speed) <= this value
#define PLAYER_LEVITATING_SPEED	4

//Gravity affects jumping velocity when jump_delay is 0
#define GRAVITY_FORCE			1

//Logic states
enum class State { IDLE, WALKING, CROUCHING, CROUCHINGATTACKING, ATTACKING, JUMPING, FALLING, CLIMBING, DEAD  };
enum class WeaponEquipped { WHIP, MORNINGSTAR, KNIFE };
enum class Equipment { WHIP, MORNINGSTAR, DAGGER, SHIELD, AXE, CROSS };

//Rendering states
enum class PlayerAnim {
	CUTSCENE,
	LEVITATING_LEFT, LEVITATING_RIGHT,
	LEVITATING_LEFT_SHIELD, LEVITATING_RIGHT_SHIELD,
	FALLING_LEFT, FALLING_RIGHT,
	FALLING_LEFT_SHIELD, FALLING_RIGHT_SHIELD,
	CLIMBING_UP_RIGHT, CLIMBING_UP_LEFT, CLIMBING_UP_RIGHT_SHIELD, CLIMBING_UP_LEFT_SHIELD,
	CLIMBING_DOWN_RIGHT, CLIMBING_DOWN_LEFT, CLIMBING_DOWN_RIGHT_SHIELD, CLIMBING_DOWN_LEFT_SHIELD,
	SHOCK_LEFT, SHOCK_RIGHT,
	TELEPORT_LEFT, TELEPORT_RIGHT,

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
	DYING_RIGHT, DYING_LEFT,

	ATTACKING_LEFT_WHIP, ATTACKING_RIGHT_WHIP,
	ATTACKING_CROUCHING_LEFT_WHIP, ATTACKING_CROUCHING_RIGHT_WHIP,
	ATTACKING_UPSTAIRS_LEFT_WHIP, ATTACKING_UPSTAIRS_RIGHT_WHIP,
	ATTACKING_DOWNSTAIRS_LEFT_WHIP, ATTACKING_DOWNSTAIRS_RIGHT_WHIP,

	ATTACKING_LEFT_MORNING_STAR, ATTACKING_RIGHT_MORNING_STAR,
	ATTACKING_CROUCHING_LEFT_MORNING_STAR, ATTACKING_CROUCHING_RIGHT_MORNING_STAR,
	ATTACKING_UPSTAIRS_LEFT_MORNING_STAR, ATTACKING_UPSTAIRS_RIGHT_MORNING_STAR,
	ATTACKING_DOWNSTAIRS_LEFT_MORNING_STAR, ATTACKING_DOWNSTAIRS_RIGHT_MORNING_STAR,

	ATTACKING_LEFT_RANGED, ATTACKING_RIGHT_RANGED,
	// DO THESE EXIST?
/*ATTACKING_CROUCHING_LEFT_RANGED, ATTACKING_CROUCHING_RIGHT_RANGED,
ATTACKING_UPSTAIRS_LEFT_RANGED, ATTACKING_UPSTAIRS_RIGHT_RANGED,
ATTACKING_DOWNSTAIRS_LEFT_RANGED, ATTACKING_DOWNSTAIRS_RIGHT_RANGED,*/

	NUM_ANIMATIONS
};

struct StepP
{
	Point speed;	//direction
	int frames;		//duration in number of frames
	int anim;		//graphical representation
};


class Player: public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();
	
	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore() const;

	void GodMode();

	int GetHP() const;
	int GetLives() const;
	void SetHearts(int heartDifference);
	int GetHearts() const;
	bool GetHasDied() const; 
	void SetHasDied(bool state);
	bool GetGameOver() const;
	void SetGameOver(bool state);
	bool GetGameEnd() const;
	void SetGameEnd(bool state);

	void SetShield();
	void SetMorningStar();

	int GetXPos();
	int GetYPos();
	AABB GetWeaponHitBox(); //const

	void GetHurt(int dmg);
	void ChangeHP(int value);

	void UpdatePattern();
	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

	Equipment SetEquipment(int equipNum);
	void GrabObject(int object);

	// Repeating these for the weapon
	bool GetPlayerIsLookingRight() const;
	bool GetPlayerIsLookingLeft() const;

	bool GetPlayerAttacking() const;
	bool GetPlayerIsInvincible() const;

	bool GetIsHoldingShield() const;

	bool GetTouchingSolidEnemy() const;
	void SetTouchingSolidEnemy(bool setter);

	void SetHasYellowKey(bool boolean);
	bool GetHasYellowKey() const;
	void SetHasWhiteKey(bool boolean);
	bool GetHasWhiteKey() const;

	int score;

	void InitPattern();
	bool patternFinished;

	void SetPlayerIsClimbing(bool boolean);
	bool GetPlayerIsClimbing() const;

private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;

	//Player mechanics
	
	void MoveX();
	void MoveY();
	void LogicJumping();
	void LogicClimbing();

	//Animation management
	void SetAnimation(int id);
	PlayerAnim GetAnimation();
	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartCrouching();
	void StartFalling();
	void StartJumping();
	void StartClimbingUp();
	void StartClimbingDown();
	void ChangeAnimRight();
	void ChangeAnimLeft();



	void InvisFrames();
	void Death();

	void finishAnimation();

	//Jump steps
	bool IsAscending() const;
	bool IsLevitating() const;
	bool IsDescending() const;

	//Ladder get in/out steps
	bool IsInFirstHalfTile() const;
	bool IsInSecondHalfTile() const;

	//Player Weapons and Items
	void Attack();
	void CrouchAttack();
	
	//void SetWeapon(Weapon w);
	////Weapon EquipWhip();
	////Weapon EquipMorningStar();
	////Weapon EquipKnife();

	Equipment EquipWhip();
	Equipment EquipMorningStar();
	Equipment EquipDagger();
	Equipment EquipShield();
	Equipment EquipAxe();
	Equipment EquipCross();

	State state;
	Look look;
	int jump_delay;
	bool isHoldingShield;
	bool isHoldingMorningStar;
	Equipment equipment;

	bool hasYellowKey;
	bool hasWhiteKey;

	TileMap *map;

	int playerTimer;
	int startingHitTimer;
	bool startingHit;

	int hp;
	int lives;
	int currentFrame;
	int hearts;

	int patternCounter;

	bool touchingSolidEnemy;

	bool hasDied;
	bool gameOver;
	bool gameEnd;

	bool wasCrouching;
	bool isInvincible;
	int startingFrame;

	bool isClimbing;

	bool isClimbingUp;
	bool isClimbingDown;
	bool startedClimbing;

	bool leftLadder;
	bool rightLadder;

	bool godMode;

	Sound chestPickupSFX;
	Sound timeTallySFX;
	Sound heartTallySFX;
	Sound whipSFX;
	Sound hitStrongEnemy;
	Sound whipHitSFX;
	Sound blockBreakingSFX;
	Sound heartSFX;
	Sound moneyBagSFX;
	Sound shieldSFX; //24
	Sound stopWatchSFX;
	Sound invisibilityOnSFX;
	Sound holyWaterSFX;
	Sound cruficixSFX;
	Sound oneUpSFX;
	Sound orbSFX;
	Sound hurtSFX;

	bool unloadedSounds;

	std::vector<StepP> pattern;
	int current_frames;
	int current_step;
	

};

