
#include "Player.h"
#include "Sprite.h"
#include "Globals.h"
#include "Weapon.h"
#include <raymath.h>
#include "SoundManager.h"
#include "raylib.h" //for sfx


Player::Player(const Point& p, State s, Look view) :
	//Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE_WIDTH, PLAYER_FRAME_SIZE_HEIGHT)
{
	state = s;
	look = view;
	jump_delay = PLAYER_JUMP_DELAY;
	map = nullptr;
	isHoldingShield = false;
	isHoldingMorningStar = false;
	hp = 32;
	lives = 3;
	score = 0;
	hearts = 0;
	hasDied = false;
	godMode = false;
	gameEnd = false;
	wasCrouching = false;
	gameOver = false;
	isInvincible = false;
	hasYellowKey = false;
	hasWhiteKey = false;
	//tmp 
	unloadedSounds = false;
	playerTimer = 0;
	startingHitTimer = 0;
	startingHit = false;
	patternCounter = 0;
	touchingSolidEnemy = false;

	isClimbingUp = false;
	isClimbingDown = false;
	startedClimbing = false;

	isClimbing = false;

	leftLadder = false;
	rightLadder = false;
}
Player::~Player()
{
}
AppStatus Player::Initialise()
{
	int i;
	const int n = PLAYER_FRAME_SIZE_WIDTH;
	const int h = PLAYER_FRAME_SIZE_HEIGHT;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_PLAYER, "img/simon.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_PLAYER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)PlayerAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_UP_RIGHT, ANIM_DELAY*2);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_UP_RIGHT, { 4 * n, 0, n, h });
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_UP_RIGHT, { 1 * n, 0, n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_UP_LEFT, ANIM_DELAY * 2);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_UP_LEFT, { 4 * n, 0, -n, h });
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_UP_LEFT, { 1 * n, 0, -n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_UP_RIGHT_SHIELD, ANIM_DELAY * 2);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_UP_RIGHT_SHIELD, { 4 * n, h, n, h });
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_UP_RIGHT_SHIELD, { 1 * n, h, n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_UP_LEFT_SHIELD, ANIM_DELAY * 2);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_UP_LEFT_SHIELD, { 4 * n, h, -n, h });
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_UP_LEFT_SHIELD, { 1 * n, h, -n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_DOWN_RIGHT, ANIM_DELAY * 2);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_DOWN_RIGHT, { 5 * n, 0, n, h });
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_DOWN_RIGHT, { 1 * n, 0, n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_DOWN_LEFT, ANIM_DELAY * 2);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_DOWN_LEFT, { 5 * n, 0, -n, h });
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_DOWN_LEFT, { 1 * n, 0, -n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_DOWN_RIGHT_SHIELD, ANIM_DELAY * 2);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_DOWN_RIGHT_SHIELD, { 5 * n, h, n, h });
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_DOWN_RIGHT_SHIELD, { 1 * n, h, n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_DOWN_LEFT_SHIELD, ANIM_DELAY * 2);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_DOWN_LEFT_SHIELD, { 5 * n, h, -n, h });
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_DOWN_LEFT_SHIELD, { 1 * n, h, -n, h });


	// Idle animations
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT, { 0, 0, -n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT, { 0, 0, n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_LEFT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT_SHIELD, { 0, h, -n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_RIGHT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT_SHIELD, { 0, h, n, h });


	// The reason this uses a for and a hard-coded line is because otherwise the animation
	// will start as Simon walking
	// Walking animations
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 1; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { (float)i * n, 0, -n, h });
	sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { (float)0 * n, 0, -n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 1; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { (float)i * n, 0, n, h });
	sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { (float)0 * n, 0, n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_LEFT_SHIELD, ANIM_DELAY);
	for (i = 1; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT_SHIELD, { (float)i * n, n * 2, -n, h });
	sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT_SHIELD, { (float)0 * n, n * 2, -n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_RIGHT_SHIELD, ANIM_DELAY);
	for (i = 1; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT_SHIELD, { (float)i * n, n * 2, n, h });
	sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT_SHIELD, { (float)0 * n, n * 2, n, h });

	// Crouching animations
	sprite->SetAnimationDelay((int)PlayerAnim::CROUCHING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CROUCHING_LEFT, { 3 * n, 0, -n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::CROUCHING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CROUCHING_RIGHT, { 3 * n, 0, n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::CROUCHING_LEFT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CROUCHING_LEFT_SHIELD, { 3 * n, 2 * n, -n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::CROUCHING_RIGHT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CROUCHING_RIGHT_SHIELD, { 3 * n, 2 * n, n, h });

	// Walking Stairs animations
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_STAIRS_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::WALKING_STAIRS_LEFT, { 1 * n, 0, -n, n * h });
	sprite->AddKeyFrame((int)PlayerAnim::WALKING_STAIRS_LEFT, { 4 * n, 0, -n, n * h });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_STAIRS_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::WALKING_STAIRS_RIGHT, { 1 * n, 0, n, h });
	sprite->AddKeyFrame((int)PlayerAnim::WALKING_STAIRS_RIGHT, { 4 * n, 0, n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_STAIRS_LEFT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::WALKING_STAIRS_LEFT_SHIELD, { 1 * n, 2 * n, -n, h });
	sprite->AddKeyFrame((int)PlayerAnim::WALKING_STAIRS_LEFT_SHIELD, { 4 * n, 2 * n, -n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_STAIRS_RIGHT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::WALKING_STAIRS_RIGHT_SHIELD, { 1 * n, 2 * n, h });
	sprite->AddKeyFrame((int)PlayerAnim::WALKING_STAIRS_RIGHT_SHIELD, { 4 * n, 2 * n, n, h });


	// Jumping animations
	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT, { 3 * n, 0, n, h });
	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT, { 3 * n, 0, -n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_RIGHT, { 3 * n, 0, n, h });
	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_LEFT, { 3 * n, 0, -n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_RIGHT, { 3 * n, 0, n, h });
	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_LEFT, { 3 * n, 0, -n, h });


	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_RIGHT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT_SHIELD, { 3 * n, h, n, h });
	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_LEFT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT_SHIELD, { 3 * n, h, -n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_RIGHT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_RIGHT_SHIELD, { 3 * n, h, n, h });
	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_LEFT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_LEFT_SHIELD, { 3 * n, h, -n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_RIGHT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_RIGHT_SHIELD, { 3 * n, h, n, h });
	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_LEFT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_LEFT_SHIELD, { 3 * n, h, -n, h });

	// Death animations
	sprite->SetAnimationDelay((int)PlayerAnim::DYING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::DYING_RIGHT, { ((float)0 + 6) * n, 0, n, h });
	sprite->AddKeyFrame((int)PlayerAnim::DYING_RIGHT, { ((float)0 + 6) * n, 0, n, h });
	sprite->AddKeyFrame((int)PlayerAnim::DYING_RIGHT, { ((float)1 + 6) * n, 0, n, h });
	sprite->AddKeyFrame((int)PlayerAnim::DYING_RIGHT, { ((float)1 + 6) * n, 0, n, h });
	for (int i = 0; i < 38; i++)
	{
		sprite->AddKeyFrame((int)PlayerAnim::DYING_RIGHT, { ((float)2 + 6) * n, 0, n * 2, h });

	}

	sprite->SetAnimationDelay((int)PlayerAnim::DYING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::DYING_LEFT, { ((float)0 + 6) * n, 0, -n, h });
	sprite->AddKeyFrame((int)PlayerAnim::DYING_LEFT, { ((float)0 + 6) * n, 0, -n, h });
	sprite->AddKeyFrame((int)PlayerAnim::DYING_LEFT, { ((float)1 + 6) * n, 0, -n, h });
	sprite->AddKeyFrame((int)PlayerAnim::DYING_LEFT, { ((float)1 + 6) * n, 0, -n, h });
	for (int i = 0; i < 38; i++)
	{
		sprite->AddKeyFrame((int)PlayerAnim::DYING_LEFT, { ((float)2 + 6) * n, 0, -n * 2 , h });
	}


	// Walking towards front animation
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_INTO, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::WALKING_INTO, { 10 * n, 0, n, h });


	// WHIP Attacking animations
	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_LEFT_WHIP, ANIM_DELAY);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_LEFT_WHIP, { (float)0 * n, n * 6, -(n * 4) , h }, -2 * n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_LEFT_WHIP, { (float)4 * n, n * 6, -(n * 4) , h }, -2 * n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_LEFT_WHIP, { (float)8 * n, n * 6, -(n * 4) , h }, -2 * n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_LEFT_WHIP, { (float)8 * n, n * 6, -(n * 4) , h }, -2 * n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_LEFT_WHIP, { (float)8 * n, n * 6, -(n * 4) , h }, -2 * n);

	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_LEFT_MORNING_STAR, ANIM_DELAY);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_LEFT_MORNING_STAR, { (float)14 * n, n * 5, -(n * 4) , h }, -2 * n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_LEFT_MORNING_STAR, { (float)18 * n, n * 5, -(n * 4) , h }, -2 * n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_LEFT_MORNING_STAR, { (float)22 * n, n * 5, -(n * 4) , h }, -2 * n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_LEFT_MORNING_STAR, { (float)22 * n, n * 5, -(n * 4) , h }, -2 * n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_LEFT_MORNING_STAR, { (float)22 * n, n * 5, -(n * 4) , h }, -2 * n);

	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_CROUCHING_LEFT_WHIP, ANIM_DELAY);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_LEFT_WHIP, { (float)0 * n, n * 8, -(n * 4) , h }, -2 * n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_LEFT_WHIP, { (float)4 * n, n * 8, -(n * 4) , h }, -2 * n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_LEFT_WHIP, { (float)8 * n, n * 8, -(n * 4) , h }, -2 * n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_LEFT_WHIP, { (float)8 * n, n * 8, -(n * 4) , h }, -2 * n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_LEFT_WHIP, { (float)8 * n, n * 8, -(n * 4) , h }, -2 * n);

	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_CROUCHING_LEFT_MORNING_STAR, ANIM_DELAY);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_LEFT_MORNING_STAR, { (float)14 * n, n * 7 + 7, -(n * 4) , h }, -2 * n, 7);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_LEFT_MORNING_STAR, { (float)18 * n, n * 7 + 7, -(n * 4) , h }, -2 * n, 7);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_LEFT_MORNING_STAR, { (float)22 * n, n * 7 + 7, -(n * 4) , h }, -2 * n, 7);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_LEFT_MORNING_STAR, { (float)22 * n, n * 7 + 7, -(n * 4) , h }, -2 * n, 7);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_LEFT_MORNING_STAR, { (float)22 * n, n * 7 + 7, -(n * 4) , h }, -2 * n, 7);

	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_UPSTAIRS_LEFT_WHIP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_UPSTAIRS_LEFT_WHIP, { (float)0 * n, n * 10, -(n * 2) , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_UPSTAIRS_LEFT_WHIP, { (float)2 * n, n * 10, -(n * 2) , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_UPSTAIRS_LEFT_WHIP, { (float)4 * n, n * 10, -(n * 4) , h });


	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_DOWNSTAIRS_LEFT_WHIP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_DOWNSTAIRS_LEFT_WHIP, { (float)0 * n, n * 12, -(n * 2) , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_DOWNSTAIRS_LEFT_WHIP, { (float)2 * n, n * 12, -(n * 2) , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_DOWNSTAIRS_LEFT_WHIP, { (float)4 * n, n * 12, -(n * 4) , h });



	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_RIGHT_WHIP, ANIM_DELAY);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_RIGHT_WHIP, { (float)0 * n, n * 6, n * 3 , h }, -n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_RIGHT_WHIP, { (float)4 * n, n * 6, n * 3 , h }, -n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_RIGHT_WHIP, { (float)8 * n, n * 6, n * 4 , h }, -n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_RIGHT_WHIP, { (float)8 * n, n * 6, n * 4 , h }, -n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_RIGHT_WHIP, { (float)8 * n, n * 6, n * 4 , h }, -n);

	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_RIGHT_MORNING_STAR, ANIM_DELAY);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_RIGHT_MORNING_STAR, { (float)14 * n, n * 5, n * 3 , h }, -n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_RIGHT_MORNING_STAR, { (float)18 * n, n * 5, n * 3 , h }, -n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_RIGHT_MORNING_STAR, { (float)22 * n, n * 5, n * 4 , h }, -n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_RIGHT_MORNING_STAR, { (float)22 * n, n * 5, n * 4 , h }, -n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_RIGHT_MORNING_STAR, { (float)22 * n, n * 5, n * 4 , h }, -n);

	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_CROUCHING_RIGHT_WHIP, ANIM_DELAY);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_RIGHT_WHIP, { (float)0 * n, n * 8, n * 3 , h }, -n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_RIGHT_WHIP, { (float)4 * n, n * 8, n * 3 , h }, -n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_RIGHT_WHIP, { (float)8 * n, n * 8, n * 4 , h }, -n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_RIGHT_WHIP, { (float)8 * n, n * 8, n * 4 , h }, -n);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_RIGHT_WHIP, { (float)8 * n, n * 8, n * 4 , h }, -n);

	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_CROUCHING_RIGHT_MORNING_STAR, ANIM_DELAY);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_RIGHT_MORNING_STAR, { (float)14 * n, n * 7 + 7, n * 3 , h }, -n, 7);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_RIGHT_MORNING_STAR, { (float)18 * n, n * 7 + 7, n * 3 , h }, -n, 7);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_RIGHT_MORNING_STAR, { (float)22 * n, n * 7 + 7, n * 4 , h }, -n, 7);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_RIGHT_MORNING_STAR, { (float)22 * n, n * 7 + 7, n * 4 , h }, -n, 7);
	sprite->AddKeyFrameOffset((int)PlayerAnim::ATTACKING_CROUCHING_RIGHT_MORNING_STAR, { (float)22 * n, n * 7 + 7, n * 4 , h }, -n, 7);

	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_UPSTAIRS_RIGHT_WHIP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_UPSTAIRS_RIGHT_WHIP, { (float)0 * n, n * 10, -(n * 2) , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_UPSTAIRS_RIGHT_WHIP, { (float)2 * n, n * 10, -(n * 2) , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_UPSTAIRS_RIGHT_WHIP, { (float)4 * n, n * 10, -(n * 4) , h });

	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_DOWNSTAIRS_RIGHT_WHIP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_DOWNSTAIRS_RIGHT_WHIP, { (float)0 * n, n * 12, -(n * 2) , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_DOWNSTAIRS_RIGHT_WHIP, { (float)2 * n, n * 12, -(n * 2) , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_DOWNSTAIRS_RIGHT_WHIP, { (float)4 * n, n * 12, -(n * 4) , h });


	//TODO: Add ranged attacks
	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_LEFT_RANGED, ANIM_DELAY);


	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_RIGHT_RANGED, ANIM_DELAY);


	// MORNING STAR Attacking animations

	//for (i = 0; i < 3; ++i)
	//	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_LEFT_WHIP, { (float)i * n, n * 6, -(n * 2) , n * 2 });

	sprite->SetAnimation((int)PlayerAnim::IDLE_RIGHT);


	// Loads all sounds:
	chestPickupSFX = data[0];
	timeTallySFX = data[3];
	heartTallySFX = data[4];
	whipSFX = data[7];
	hitStrongEnemy = data[15];
	whipHitSFX = data[19];
	blockBreakingSFX = data[20];
	heartSFX = data[21];
	moneyBagSFX = data[22];
	shieldSFX = data[23];
	stopWatchSFX = data[25];
	invisibilityOnSFX = data[26];
	holyWaterSFX = data[29];
	cruficixSFX = data[34];
	oneUpSFX = data[35];
	hurtSFX = data[36];


	return AppStatus::OK;
}
void Player::InitScore()
{
	score = 0;
}
void Player::IncrScore(int n)
{
	score += n;
}
void Player::GodMode()
{
	godMode = !godMode;
}
int Player::GetScore() const
{
	return score;
}
int Player::GetLives() const
{
	return lives;
}
bool Player::GetHasDied() const
{
	return hasDied;
}
void Player::SetHasDied(bool state)
{
	hasDied = state;
}
bool Player::GetGameOver() const
{
	return gameOver;
}
void Player::SetGameOver(bool state)
{
	gameOver = state;
}
bool Player::GetGameEnd() const
{
	return gameEnd;
}
void Player::SetGameEnd(bool state)
{
	gameEnd = state;
}
int Player::GetHP() const
{
	return hp;
}
void Player::SetHearts(int heartDifference)
{
	if (heartDifference > 0)
		PlaySound(heartSFX);
	hearts += heartDifference;
}
int Player::GetHearts() const
{
	return hearts;
}
void Player::SetShield()
{
	isHoldingShield = true;
}
void Player::SetMorningStar()
{
	isHoldingMorningStar = true;
}
int Player::GetXPos()
{
	return pos.x;
}
int Player::GetYPos()
{
	return pos.y;
}
// TODO Delay hitbox to be on the last frame, but I'm not sure how.
AABB Player::GetWeaponHitBox()
{
	if (state == State::ATTACKING && look == Look::RIGHT && isHoldingMorningStar && !wasCrouching)
	{
		if (startingHit == false)
		{
			startingHit = true;
			startingHitTimer = playerTimer;
		}
		else if (startingHit == true && startingHitTimer == playerTimer)
		{
			startingHit = true;
			AABB hitbox({ pos.x + width, pos.y - height / 2 - 8 }, width * 3, height / 3);
			return hitbox;
		}
		Point p(0, 0);
		AABB hitbox(p, 0, 0);
		return hitbox;
	}
	else if (state == State::ATTACKING && look == Look::LEFT && isHoldingMorningStar && !wasCrouching)
	{
		if (startingHit == false)
		{
			startingHit = true;
			startingHitTimer = playerTimer;
		}
		else if (startingHit == true && startingHitTimer == playerTimer)
		{
			startingHit = true;
			AABB hitbox({ pos.x - (width * 3), pos.y - height / 2 - 8 }, width * 3, height / 3);
			return hitbox;
		}
		Point p(0, 0);
		AABB hitbox(p, 0, 0);
		return hitbox;
	}
	else if (state == State::ATTACKING && look == Look::RIGHT && isHoldingMorningStar && wasCrouching)
	{
		if (startingHit == false)
		{
			startingHit = true;
			startingHitTimer = playerTimer;
		}
		else if (startingHit == true && startingHitTimer == playerTimer)
		{
			startingHit = true;
			AABB hitbox({ pos.x + width, pos.y - height / 3 }, width * 3, height / 3);
			return hitbox;
		}
		Point p(0, 0);
		AABB hitbox(p, 0, 0);
		return hitbox;
	}
	else if (state == State::ATTACKING && look == Look::LEFT && isHoldingMorningStar && wasCrouching)
	{
		if (startingHit == false)
		{
			startingHit = true;
			startingHitTimer = playerTimer;
		}
		else if (startingHit == true && startingHitTimer == playerTimer)
		{
			startingHit = true;
			AABB hitbox({ pos.x - (width * 3), pos.y - height / 3 }, width * 3, height / 3);
			return hitbox;
		}
		Point p(0, 0);
		AABB hitbox(p, 0, 0);
		return hitbox;
	}
	else if (state == State::ATTACKING && look == Look::RIGHT && wasCrouching)
	{
		if (startingHit == false)
		{
			startingHit = true;
			startingHitTimer = playerTimer;
		}
		else if (startingHit == true && startingHitTimer == playerTimer)
		{
			startingHit = true;
			AABB hitbox({ pos.x + width, pos.y - height / 3 }, width * 2.2, height / 3);
			return hitbox;
		}
		Point p(0, 0);
		AABB hitbox(p, 0, 0);
		return hitbox;
	}
	else if (state == State::ATTACKING && look == Look::LEFT && wasCrouching)
	{
		if (startingHit == false)
		{
			startingHit = true;
			startingHitTimer = playerTimer;
		}
		else if (startingHit == true && startingHitTimer == playerTimer)
		{
			startingHit = true;
			AABB hitbox({ pos.x - (width * 2/*.1*/), pos.y - height / 3 }, width * 2.2, height / 3);
			return hitbox;
		}
		Point p(0, 0);
		AABB hitbox(p, 0, 0);
		return hitbox;
	}
	else if (state == State::ATTACKING && look == Look::RIGHT && !wasCrouching)
	{
		if (startingHit == false)
		{
			startingHit = true;
			startingHitTimer = playerTimer;
		}
		else if (startingHit == true && startingHitTimer == playerTimer)
		{
			startingHit = true;
			AABB hitbox({ pos.x + width, pos.y - height / 2 - 8 }, width * 2.2, height / 3);
			return hitbox;
		}
		Point p(0, 0);
		AABB hitbox(p, 0, 0);
		return hitbox;
	}
	else if (state == State::ATTACKING && look == Look::LEFT && !wasCrouching)
	{
		if (startingHit == false)
		{
			startingHit = true;
			startingHitTimer = playerTimer;
		}
		else if (startingHit == true && startingHitTimer == playerTimer)
		{
			startingHit = true;
			AABB hitbox({ pos.x - (width * 2/*.1*/), pos.y - height / 2 - 8 }, width * 2.2, height / 3);
			return hitbox;
		}
		Point p(0, 0);
		AABB hitbox(p, 0, 0);
		return hitbox;
	}
	else
	{
		Point p(0, 0);
		AABB hitbox(p, 0, 0);
		return hitbox;
	}
}
void Player::GetHurt(int dmg)
{
	if (!godMode && !isInvincible && hp > 0)
	{
		
		if (hp > 0)
		{
			hp -= dmg;
			if (hp <= 0)
			{
				hp = 0;
				Death();
			}
			else
			{
				PlaySound(hurtSFX);
				InvisFrames();
			}
			
		}
		/*hp += value;
		if (hp > 100)
			hp = 100;
		if (hp <= 0)
		{
			hp = 0;
			Death();
		}*/
	}
}
bool Player::GetIsHoldingShield() const
{
	return Player::isHoldingShield;
}
bool Player::GetTouchingSolidEnemy() const
{
	return touchingSolidEnemy;
}
void Player::SetTouchingSolidEnemy(bool setter)
{
	touchingSolidEnemy = setter;
}
void Player::SetHasYellowKey(bool boolean)
{
	hasYellowKey = boolean;
}
bool Player::GetHasYellowKey() const
{
	return hasYellowKey;
}
void Player::SetHasWhiteKey(bool boolean)
{
	hasWhiteKey = boolean;
}
bool Player::GetHasWhiteKey() const
{
	return hasWhiteKey;
}
Equipment Player::SetEquipment(int equipNum)
{
	if (equipNum == 0)
	{
		return EquipShield();
	}
	else if (equipNum == 100)
	{
		return EquipMorningStar();
	}
}
void Player::GrabObject(int object)
{
	// This case breaks the sfx and makes it play continuously
	switch (object)
	{
	case 1:
		PlaySound(moneyBagSFX);
		break;
	//case 2:
	//	
	//	PlaySound(orbSFX);
	//	break;
	case 20:
		PlaySound(chestPickupSFX);
		break;
	default:
		break;
	}
}
Equipment Player::EquipWhip()
{
	return equipment = Equipment::WHIP;
}
Equipment Player::EquipMorningStar()
{
	isHoldingMorningStar = true;

	PlaySound(shieldSFX);   // temporal approch
	return equipment = Equipment::MORNINGSTAR;
}
Equipment Player::EquipDagger()
{
	return equipment = Equipment::DAGGER;
}
Equipment Player::EquipShield()
{
	isHoldingShield = true;
	Stop();
	
	PlaySound(shieldSFX);   // temporal approch
	return equipment = Equipment::SHIELD;
}
Equipment Player::EquipAxe()
{
	return equipment = Equipment::AXE;
}
Equipment Player::EquipCross()
{
	return equipment = Equipment::CROSS;
}
void Player::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Player::GetPlayerIsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Player::GetPlayerIsLookingLeft() const
{
	return look == Look::LEFT;
}
bool Player::GetPlayerAttacking() const
{
	if (state == State::ATTACKING)
		return true;
	else
		return false;
}
bool Player::GetPlayerIsInvincible() const
{
	if (isInvincible)
		return true;
	else
		return false;
}
bool Player::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Player::IsLookingLeft() const
{
	return look == Look::LEFT;
}
bool Player::IsAscending() const
{
	return dir.y < -PLAYER_LEVITATING_SPEED;
}
bool Player::IsLevitating() const
{
	return abs(dir.y) <= PLAYER_LEVITATING_SPEED;
}
bool Player::IsDescending() const
{
	return dir.y > PLAYER_LEVITATING_SPEED;
}
bool Player::IsInFirstHalfTile() const
{
	return pos.y % TILE_SIZE < TILE_SIZE/2;
}
bool Player::IsInSecondHalfTile() const
{
	return pos.y % TILE_SIZE >= TILE_SIZE / 2;
}
void Player::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
PlayerAnim Player::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (PlayerAnim)sprite->GetAnimation();
}
void Player::Stop()
{
	dir = { 0,0 };
	state = State::IDLE;
	if (IsLookingRight())
	{
		if (GetIsHoldingShield() == true)
		{
			SetAnimation((int)PlayerAnim::IDLE_RIGHT_SHIELD);
		}
		else
		{
			SetAnimation((int)PlayerAnim::IDLE_RIGHT);
		}
	}
	else
	{
		if (GetIsHoldingShield() == true)
		{
			SetAnimation((int)PlayerAnim::IDLE_LEFT_SHIELD);
		}
		else
		{
			SetAnimation((int)PlayerAnim::IDLE_LEFT);
		}
	}
}
void Player::StartWalkingLeft()
{
	state = State::WALKING;
	look = Look::LEFT;
	if (GetIsHoldingShield() == true)
		SetAnimation((int)PlayerAnim::WALKING_LEFT_SHIELD);
	else 
		SetAnimation((int)PlayerAnim::WALKING_LEFT);
}
void Player::StartWalkingRight()
{
	state = State::WALKING;
	look = Look::RIGHT;
	if (GetIsHoldingShield() == true)
		SetAnimation((int)PlayerAnim::WALKING_RIGHT_SHIELD);
	else
		SetAnimation((int)PlayerAnim::WALKING_RIGHT);
}
void Player::StartCrouching()
{
	state = State::CROUCHING;
	if (IsLookingRight())
	{
		if (GetIsHoldingShield() == true)
			SetAnimation((int)PlayerAnim::CROUCHING_RIGHT_SHIELD);
		else
		SetAnimation((int)PlayerAnim::CROUCHING_RIGHT);
	}
	else
	{
		if (GetIsHoldingShield() == true)
			SetAnimation((int)PlayerAnim::CROUCHING_LEFT_SHIELD);
		else
			SetAnimation((int)PlayerAnim::CROUCHING_LEFT);
	}
}
void Player::StartFalling()
{
	dir.y = PLAYER_SPEED;
	state = State::FALLING;
	if (IsLookingRight())
	{
		if (GetIsHoldingShield() == true)
			SetAnimation((int)PlayerAnim::FALLING_RIGHT_SHIELD);
		else
			SetAnimation((int)PlayerAnim::FALLING_RIGHT);

	}
	else
	{
		if (GetIsHoldingShield() == true)
			SetAnimation((int)PlayerAnim::FALLING_LEFT_SHIELD);
		else
			SetAnimation((int)PlayerAnim::FALLING_LEFT);
	}
}
void Player::StartJumping()
{
	dir.y = -PLAYER_JUMP_FORCE;
	state = State::JUMPING;
	if (IsLookingRight())
	{
		if (GetIsHoldingShield() == true)
			SetAnimation((int)PlayerAnim::JUMPING_RIGHT_SHIELD);
		else
			SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
	}
	else
	{
		if (GetIsHoldingShield() == true)
			SetAnimation((int)PlayerAnim::JUMPING_LEFT_SHIELD);
		else
			SetAnimation((int)PlayerAnim::JUMPING_LEFT);
	}
	jump_delay = PLAYER_JUMP_DELAY;
}
void Player::StartClimbingUp()
{
	state = State::CLIMBING;
	if (look == Look::RIGHT && isHoldingShield)
		SetAnimation((int)PlayerAnim::CLIMBING_UP_RIGHT_SHIELD);
	else if (look == Look::RIGHT && !isHoldingShield)
		SetAnimation((int)PlayerAnim::CLIMBING_UP_RIGHT);
	else if (look == Look::LEFT && isHoldingShield)
		SetAnimation((int)PlayerAnim::CLIMBING_UP_LEFT_SHIELD);
	else if (look == Look::LEFT && !isHoldingShield)
		SetAnimation((int)PlayerAnim::CLIMBING_UP_LEFT);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}
void Player::StartClimbingDown()
{
	state = State::CLIMBING;
	if (look == Look::RIGHT && isHoldingShield)
		SetAnimation((int)PlayerAnim::CLIMBING_DOWN_RIGHT_SHIELD);
	else if (look == Look::RIGHT && !isHoldingShield)
		SetAnimation((int)PlayerAnim::CLIMBING_DOWN_RIGHT);
	else if (look == Look::LEFT && isHoldingShield)
		SetAnimation((int)PlayerAnim::CLIMBING_DOWN_LEFT_SHIELD);
	else if (look == Look::LEFT && !isHoldingShield)
		SetAnimation((int)PlayerAnim::CLIMBING_DOWN_LEFT);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}
void Player::Attack()
{
	if (state == State::CROUCHING)
	{
		wasCrouching = true;
		state = State::ATTACKING;
		if (look == Look::RIGHT)
		{
			if (isHoldingMorningStar)
			{
				SetAnimation((int)PlayerAnim::ATTACKING_CROUCHING_RIGHT_MORNING_STAR);
				Sprite* sprite = dynamic_cast<Sprite*>(render);
				sprite->SetPlayOnceMode();
			}
			else
			{
				SetAnimation((int)PlayerAnim::ATTACKING_CROUCHING_RIGHT_WHIP);
				Sprite* sprite = dynamic_cast<Sprite*>(render);
				sprite->SetPlayOnceMode();
			}
		}
		else if (look == Look::LEFT)
		{
			if (isHoldingMorningStar)
			{
				SetAnimation((int)PlayerAnim::ATTACKING_CROUCHING_LEFT_MORNING_STAR);
				Sprite* sprite = dynamic_cast<Sprite*>(render);
				sprite->SetPlayOnceMode();
			}
			else
			{
				SetAnimation((int)PlayerAnim::ATTACKING_CROUCHING_LEFT_WHIP);
				Sprite* sprite = dynamic_cast<Sprite*>(render);
				sprite->SetPlayOnceMode();
			}
		}
	}

	else 
	{
		state = State::ATTACKING;
		if (look == Look::RIGHT)
		{
			if (isHoldingMorningStar)
			{
				SetAnimation((int)PlayerAnim::ATTACKING_RIGHT_MORNING_STAR);
				Sprite* sprite = dynamic_cast<Sprite*>(render);
				sprite->SetPlayOnceMode();
			}
			else
			{
				SetAnimation((int)PlayerAnim::ATTACKING_RIGHT_WHIP);
				Sprite* sprite = dynamic_cast<Sprite*>(render);
				sprite->SetPlayOnceMode();
			}
		}
		else if (look == Look::LEFT)
		{
			if (isHoldingMorningStar)
			{
				SetAnimation((int)PlayerAnim::ATTACKING_LEFT_MORNING_STAR);
				Sprite* sprite = dynamic_cast<Sprite*>(render);
				sprite->SetPlayOnceMode();
			}
			else
			{
				SetAnimation((int)PlayerAnim::ATTACKING_LEFT_WHIP);
				Sprite* sprite = dynamic_cast<Sprite*>(render);
				sprite->SetPlayOnceMode();
			}
		}
	}
	
	//TODO: Add attacking SFX
	// This works, but it will cause a memory leak.
	//sfxList[8] = LoadSound("sfx/08.wav");
	PlaySound(whipSFX);
	
	
}

void Player::ChangeHP(int value)
{
	if (!godMode && !isInvincible)
	{
		hp += value;
		if (hp > 32)
			hp = 32;
		if (hp <= 0)
		{
			hp = 0;
		}
	}		
}

void Player::SetPlayerIsClimbing(bool boolean)
{
	isClimbing = boolean;
}
bool Player::GetPlayerIsClimbing() const
{
	return isClimbing;
}

void Player::InvisFrames()
{
	startingFrame = currentFrame;
	isInvincible = true;
	/*if (isInvincible == false)
	{
		startingFrame = currentFrame;
		isInvincible = true;
	}
	
	if (currentFrame - startingFrame  < 100)
	{
		godMode = true;
	}
	else if (currentFrame - startingFrame >= 100)
	{
		godMode = false;
		isInvincible = false;
	}*/

	//{
	//	godMode = true;
	//}
	//godMode = false;
}

void Player::Death()
{
	/*Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetPlayOnceMode();*/
	hasDied = true;
	state = State::DEAD;
	lives--;
	if (look == Look::RIGHT)
	{
		SetAnimation((int)PlayerAnim::DYING_RIGHT);
		Sprite* sprite = dynamic_cast<Sprite*>(render);
		//while (sprite->GetIsAnimationFinished() == false)
		//{
		sprite->SetPlayOnceMode();
	}
	else if (look == Look::LEFT)
	{
		SetAnimation((int)PlayerAnim::DYING_LEFT);
		Sprite* sprite = dynamic_cast<Sprite*>(render);
		//while (sprite->GetIsAnimationFinished() == false)
		//{
		sprite->SetPlayOnceMode();
	}

	if (lives <= 0)
	{
		gameOver = true;
		
	}
	else
	{
		// unsure where the wait time must be
		// TODO: Fix this shit bro
		//reset screen
		
	}
}
//void Player::finishAnimation()
//{
//	Sprite* sprite = dynamic_cast<Sprite*>(render); //Se intent� pero no tengo ni idea de c�mo definir la animaci�n de dead como playonce para que el bool sea true (ToT)
//	if (sprite->isAnimationFinished == true) 
//	{
//		SetAnimation((int)PlayerAnim::IDLE_RIGHT);
//		pos.x = 0;
//	}
//}
void Player::ChangeAnimRight()
{
	look = Look::RIGHT;
	switch (state)
	{
		case State::IDLE:	 
			if (GetIsHoldingShield() == true)
				SetAnimation((int)PlayerAnim::IDLE_RIGHT_SHIELD);
			else 
				SetAnimation((int)PlayerAnim::IDLE_RIGHT);
			    break; 
		case State::WALKING:
			if (GetIsHoldingShield() == true)
				SetAnimation((int)PlayerAnim::WALKING_RIGHT_SHIELD); 
			else
				SetAnimation((int)PlayerAnim::WALKING_RIGHT);
			break;
		case State::JUMPING: 
			if (GetIsHoldingShield() == true)
				SetAnimation((int)PlayerAnim::JUMPING_RIGHT_SHIELD); 
			else
				SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
				break;
		case State::FALLING:
			if (GetIsHoldingShield() == true)
				SetAnimation((int)PlayerAnim::FALLING_RIGHT_SHIELD);
			else
				SetAnimation((int)PlayerAnim::FALLING_RIGHT);
			break;
	}
}
void Player::ChangeAnimLeft()
{
	look = Look::LEFT;
	switch (state)
	{
		if (GetIsHoldingShield() == true)
			SetAnimation((int)PlayerAnim::IDLE_LEFT_SHIELD);
		else
			SetAnimation((int)PlayerAnim::IDLE_LEFT);
		break;
	case State::WALKING:
		if (GetIsHoldingShield() == true)
			SetAnimation((int)PlayerAnim::WALKING_LEFT_SHIELD);
		else
			SetAnimation((int)PlayerAnim::WALKING_LEFT);
		break;
	case State::JUMPING:
		if (GetIsHoldingShield() == true)
			SetAnimation((int)PlayerAnim::JUMPING_LEFT_SHIELD);
		else
			SetAnimation((int)PlayerAnim::JUMPING_LEFT);
		break;
		case State::FALLING: 
			if (GetIsHoldingShield() == true)
				SetAnimation((int)PlayerAnim::FALLING_LEFT_SHIELD);
			else
				SetAnimation((int)PlayerAnim::FALLING_LEFT);
			break;
	}
}
void Player::Update()
{
	playerTimer++;
	playerTimer %= 7;
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	currentFrame++;
	currentFrame %= 100;
	if (isInvincible == true)
	{
		if (currentFrame == startingFrame)
		{
			isInvincible = false;
		}
	}

	if (IsKeyPressed(KEY_SPACE) && state != State::ATTACKING && hasDied == false)
	{
		Attack();
	}

	MoveX();
	MoveY();
	
	if (IsKeyPressed(KEY_F5))
	{
		if (!isHoldingShield)
			PlaySound(shieldSFX);
		isHoldingShield = !isHoldingShield;
		Stop();
	}
	if (IsKeyPressed(KEY_F6))
	{
		ChangeHP(-10);
	}
	if (IsKeyPressed(KEY_F7))
	{
		ChangeHP(+10);
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();

	

	if (sprite->GetIsAnimationFinished())
	{
		sprite->SetIsAnimationFinished(false);
		if (wasCrouching)
		{

			state = State::CROUCHING;
			StartCrouching();
		}
		else
		{
			state = State::IDLE;
			Stop();

		}
	
		wasCrouching = false;
		//Stop();
		
		
		//state = State::IDLE;
		
	}
}
void Player::InitPattern()
{

	int i;
	const int n = PLAYER_FRAME_SIZE_WIDTH;
	const int h = PLAYER_FRAME_SIZE_HEIGHT;
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	look = Look::LEFT;
	sprite->SetAnimationDelay((int)PlayerAnim::CUTSCENE, ANIM_DELAY);

	//Times for the walking animation to be repeated
	for (int i = 0; i < 7; i++)
	{
		sprite->AddKeyFrame((int)PlayerAnim::CUTSCENE, { (float)0 * n, 0, -n, h });
		sprite->AddKeyFrame((int)PlayerAnim::CUTSCENE, { (float)0 * n, 0, -n, h });
		sprite->AddKeyFrame((int)PlayerAnim::CUTSCENE, { (float)1 * n, 0, -n, h });
		sprite->AddKeyFrame((int)PlayerAnim::CUTSCENE, { (float)1 * n, 0, -n, h });
		sprite->AddKeyFrame((int)PlayerAnim::CUTSCENE, { (float)2 * n, 0, -n, h });
		sprite->AddKeyFrame((int)PlayerAnim::CUTSCENE, { (float)2 * n, 0, -n, h });
		sprite->AddKeyFrame((int)PlayerAnim::CUTSCENE, { (float)1 * n, 0, -n, h });
		sprite->AddKeyFrame((int)PlayerAnim::CUTSCENE, { (float)1 * n, 0, -n, h });
	}
	sprite->AddKeyFrame((int)PlayerAnim::CUTSCENE, { (float)0 * n, 0, -n, h });

	for (int i = 0; i < 11; i++)
	{
		sprite->AddKeyFrame((int)PlayerAnim::CUTSCENE, { (float)10 * n, 0, -n, h });
	}

	sprite->SetAnimation((int)PlayerAnim::CUTSCENE);

}
void Player::UpdatePattern()
{
	patternCounter++;
	if (pos.x > 128)
		if (patternCounter % 2 == 0)
			pos.x = pos.x - 1;
	/*Sprite* sprite = dynamic_cast<Sprite*>(render);
	int anim_id;


	current_frames++;
	pos += pattern[current_step].speed;
	if (current_frames == pattern[current_step].frames)
	{
		patternFinished = true;
	}*/
	//SetAnimation((int)PlayerAnim::CUTSCENE);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	if (pos.x > 128)
		sprite->Update();
	
}
void Player::MoveX()
{
	AABB box;
	int prev_x = pos.x;

	//We can only go up and down while climbing
	//if (state == State::CLIMBING)	return;
	if (IsKeyPressed(KEY_L))
	{
		GetHurt(32);
	}
	if (state == State::CLIMBING) 
	{
		return;
	}
	if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && hasDied == false && state != State::ATTACKING)
	{
		if (state == State::IDLE) StartCrouching();
		else if (state == State::WALKING) StartCrouching();
		else if (state == State::JUMPING) return;
	}
	else if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && !IsKeyDown(KEY_RIGHT) && hasDied == false && state != State::ATTACKING)
	{
		pos.x += -PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingLeft();
		else if (state == State::CROUCHING) StartWalkingLeft();
		else
		{
			if (IsLookingRight()) ChangeAnimLeft();
		}

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box) || pos.x < 0 || touchingSolidEnemy)
		{
			
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
			if (touchingSolidEnemy)
			{
				pos.x+=2;
				touchingSolidEnemy = false;
			}
		}
	}
	else if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && hasDied == false && state != State::ATTACKING)
	{
		pos.x += PLAYER_SPEED;
		if (state == State::IDLE)
			StartWalkingRight();
		else if (state == State::CROUCHING) StartWalkingRight();
		else
		{
			if (IsLookingLeft()) ChangeAnimRight();
		}

		box = GetHitbox();
		if (map->TestCollisionWallRight(box) || pos.x > (WINDOW_WIDTH - width + 1) || touchingSolidEnemy)
		{
			
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
			if (touchingSolidEnemy)
			{
				pos.x-=2;
				touchingSolidEnemy = false;
			}
		}
	}
	else
	{
		if (state == State::WALKING) Stop();
		else if (state == State::CROUCHING) Stop();
	}
}
void Player::MoveY()
{
	AABB box;
	

	if (state == State::JUMPING && state != State::ATTACKING)
	{
		LogicJumping();
		isClimbing = false;
	}
	else if (state == State::CLIMBING && state != State::ATTACKING)
	{
		LogicClimbing();
	}
	else //idle, walking, falling
	{
		isClimbing = false;
		pos.y += PLAYER_SPEED;
		box = GetHitbox();


		if (map->TestCollisionGround(box, &pos.y))
		{

			/*if (state == State::ATTACKING && state != State::ATTACKING)
				Attack();*/

			if (state == State::FALLING && state != State::ATTACKING) Stop();
			else if (((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && !map->TestOnLadder(box, &pos.x)))
				StartJumping();
			else if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && state != State::ATTACKING && hasDied == false)
			{
				box = GetHitbox();
				if (map->TestOnLadder(box, &pos.x))
				{
					/*if (map->TestOnLadderBotLeft(box, &pos.x))
					{
						leftLadder = true;
						rightLadder = false;
					}
					else if (map->TestOnLadderBotRight(box, &pos.x))
					{
						rightLadder = true;

					}*/
					StartClimbingUp();
				}
				//else if ()
				//{
				//	StartJumping();
				//}
			}
			else if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && state != State::ATTACKING && hasDied == false)
			{
				//To climb up the ladder, we need to check the control point (x, y)
				//To climb down the ladder, we need to check pixel below (x, y+1) instead
				box = GetHitbox();
				box.pos.y++;
				if (map->TestOnLadderTop(box, &pos.x) && !startedClimbing) //This has to be TestOnLadderTop()
				{
					if (map->TestOnLadderTopRight(box, &pos.x))
					{
						look = Look::LEFT;
						//pos.y += 16;
						//pos.x -= 16;
					}
					else if (map->TestOnLadderTopLeft(box, &pos.x))
					{
						look = Look::RIGHT;
						//pos.y += 16;
						//pos.x += 16;
					}
					StartClimbingDown();
				}
				else
				{
					StartCrouching();
				}
					
			}
		}
		else
		{
			if (state != State::FALLING && state != State::ATTACKING) StartFalling();
		}
	}
}
void Player::LogicJumping()
{
	AABB box, prev_box;
	int prev_y;

	jump_delay--;
	if (jump_delay == 0)
	{
		prev_y = pos.y;
		prev_box = GetHitbox();

		pos.y += dir.y;
		dir.y += GRAVITY_FORCE;
		jump_delay = PLAYER_JUMP_DELAY;

		//Is the jump finished?
		if (dir.y > PLAYER_JUMP_FORCE)
		{
			dir.y = PLAYER_SPEED;
			StartFalling();
		}
		else
		{
			//Jumping is represented with 3 different states
			if (IsAscending())
			{
				if (GetIsHoldingShield() == true)
				{
					if (IsLookingRight())
						SetAnimation((int)PlayerAnim::JUMPING_RIGHT_SHIELD);
					else
						SetAnimation((int)PlayerAnim::JUMPING_LEFT_SHIELD);
				}
				else
				{
					if (IsLookingRight())
						SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
					else
						SetAnimation((int)PlayerAnim::JUMPING_LEFT);
				}
			}
			else if (IsLevitating())
			{
				if (GetIsHoldingShield() == true)
				{
					if (IsLookingRight())
						SetAnimation((int)PlayerAnim::JUMPING_RIGHT_SHIELD);
					else
						SetAnimation((int)PlayerAnim::JUMPING_LEFT_SHIELD);
				}
				else
				{
					if (IsLookingRight())
						SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
					else
						SetAnimation((int)PlayerAnim::JUMPING_LEFT);
				}
			}
			else if (IsDescending())
			{
				if (GetIsHoldingShield() == true)
				{
					if (IsLookingRight())
						SetAnimation((int)PlayerAnim::JUMPING_RIGHT_SHIELD);
					else
						SetAnimation((int)PlayerAnim::JUMPING_LEFT_SHIELD);
				}
				else
				{
					if (IsLookingRight())
						SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
					else
						SetAnimation((int)PlayerAnim::JUMPING_LEFT);
				}
			}
		}
		//We check ground collision when jumping down
		if (dir.y >= 0)
		{
			box = GetHitbox();

			//A ground collision occurs if we were not in a collision state previously.
			//This prevents scenarios where, after levitating due to a previous jump, we found
			//ourselves inside a tile, and the entity would otherwise be placed above the tile,
			//crossing it.
			if (!map->TestCollisionGround(prev_box, &prev_y) &&
				map->TestCollisionGround(box, &pos.y))
			{
				Stop();
			}
		}
	}
}
void Player::LogicClimbing()
{
	isClimbing = true;
	AABB box;
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	int tmp;

	if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
	{
		isClimbingUp = true;
		isClimbingDown = false;
		look = Look::RIGHT;
		pos.y -= PLAYER_LADDER_SPEED;
		pos.x += PLAYER_LADDER_SPEED;
		//pos.x -= PLAYER_LADDER_SPEED;
		sprite->NextFrame();
	}
	else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
	{
		isClimbingUp = false;
		isClimbingDown = true;
		look = Look::LEFT;
		pos.y += PLAYER_LADDER_SPEED;
		pos.x -= PLAYER_LADDER_SPEED;
		//pos.x += PLAYER_LADDER_SPEED;
		sprite->NextFrame(); // Change the sprite to go downwards, check what sprite is
	}
	//if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && state != State::ATTACKING && look == Look::RIGHT)
	//{
	//	isClimbingUp = true;
	//	isClimbingDown = false;
	//	pos.y -= PLAYER_LADDER_SPEED;
	//	pos.x += PLAYER_LADDER_SPEED;
	//	sprite->NextFrame();
	//}
	//else if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && state != State::ATTACKING && look == Look::LEFT)
	//{
	//	isClimbingUp = true;
	//	isClimbingDown = false;
	//	pos.y -= PLAYER_LADDER_SPEED;
	//	pos.x -= PLAYER_LADDER_SPEED;
	//	sprite->NextFrame();
	//}
	//else if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && state != State::ATTACKING && look == Look::RIGHT)
	//{
	//	isClimbingUp = false;
	//	isClimbingDown = true;
	//	pos.y += PLAYER_LADDER_SPEED;
	//	pos.x += PLAYER_LADDER_SPEED;
	//	sprite->NextFrame(); // Change the sprite to go downwards, check what sprite is
	//}
	//else if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && state != State::ATTACKING && look == Look::LEFT)
	//{
	//	isClimbingUp = false;
	//	isClimbingDown = true;
	//	pos.y += PLAYER_LADDER_SPEED;
	//	pos.x -= PLAYER_LADDER_SPEED;
	//	sprite->NextFrame(); // Change the sprite to go downwards, check what sprite is
	//}

	//It is important to first check LadderTop due to its condition as a collision ground.
	//By doing so, we ensure that we don't stop climbing down immediately after starting the descent.
	box = GetHitbox();

	//if (map->TestCollisionGround(box, &pos.y))
	//{
	//	//Case leaving the ladder descending.
	//	Stop();
	//	sprite->SetAutomaticMode();
	//}
	//else 
		if (!map->TestOnLadder(box, &tmp) /*&& !map->TestOnLadderTop(box, &tmp)*/)
	{
		//Case leaving the ladder ascending.
		//If we are not in a LadderTop, colliding ground or in the Ladder it means we are leaving
		//ther ladder ascending.
		Stop();
		sprite->SetAutomaticMode();
	}
	else
	{
		if (isClimbingUp && look == Look::RIGHT && GetAnimation() != PlayerAnim::CLIMBING_UP_RIGHT_SHIELD && isHoldingShield)
			SetAnimation((int)PlayerAnim::CLIMBING_UP_RIGHT_SHIELD);
		else if (isClimbingUp && look == Look::RIGHT && GetAnimation() != PlayerAnim::CLIMBING_UP_RIGHT && !isHoldingShield)
			SetAnimation((int)PlayerAnim::CLIMBING_UP_RIGHT);
		else if (isClimbingUp && look == Look::LEFT && GetAnimation() != PlayerAnim::CLIMBING_UP_LEFT_SHIELD && isHoldingShield)
			SetAnimation((int)PlayerAnim::CLIMBING_UP_LEFT_SHIELD);
		else if (isClimbingUp && look == Look::LEFT && GetAnimation() != PlayerAnim::CLIMBING_UP_LEFT && !isHoldingShield)
			SetAnimation((int)PlayerAnim::CLIMBING_UP_LEFT);
		else if (isClimbingDown && look == Look::RIGHT && GetAnimation() != PlayerAnim::CLIMBING_DOWN_RIGHT_SHIELD && isHoldingShield)
			SetAnimation((int)PlayerAnim::CLIMBING_DOWN_RIGHT_SHIELD);
		else if (isClimbingDown && look == Look::RIGHT && GetAnimation() != PlayerAnim::CLIMBING_DOWN_RIGHT && !isHoldingShield)
			SetAnimation((int)PlayerAnim::CLIMBING_DOWN_RIGHT);
		else if (isClimbingDown && look == Look::LEFT && GetAnimation() != PlayerAnim::CLIMBING_DOWN_LEFT_SHIELD && isHoldingShield)
			SetAnimation((int)PlayerAnim::CLIMBING_DOWN_LEFT_SHIELD);
		else if (isClimbingDown && look == Look::LEFT && GetAnimation() != PlayerAnim::CLIMBING_DOWN_LEFT && !isHoldingShield)
			SetAnimation((int)PlayerAnim::CLIMBING_DOWN_LEFT);
	}
}
void Player::DrawDebug(const Color& col) const
{	
	if (state == State::CROUCHING || wasCrouching || state == State::JUMPING)
		Entity::DrawHitbox(pos.x, pos.y, width, height - height / 3, col);
	else
		Entity::DrawHitbox(pos.x, pos.y, width, height, col);
	
	if (state == State::ATTACKING && look == Look::RIGHT && isHoldingMorningStar && !wasCrouching)
	{
		Entity::DrawHitbox(pos.x + width, pos.y - height / 2, width * 3, height / 3, col);
	}
	else if (state == State::ATTACKING && look == Look::LEFT && isHoldingMorningStar && !wasCrouching)
	{
		Entity::DrawHitbox(pos.x - (width * 3), pos.y - height / 2, width * 3, height / 3, col);
	}
	else if (state == State::ATTACKING && look == Look::RIGHT && isHoldingMorningStar && wasCrouching)
	{
		Entity::DrawHitbox(pos.x + width, pos.y - height / 3, width * 3, height / 3, col);
	}
	else if (state == State::ATTACKING && look == Look::LEFT && isHoldingMorningStar && wasCrouching)
	{
		Entity::DrawHitbox(pos.x - (width * 3), pos.y - height / 3, width * 3, height / 3, col);
	}
	else if (state == State::ATTACKING && look == Look::RIGHT && wasCrouching)
	{
		Entity::DrawHitbox(pos.x + width, pos.y - height / 3, width * 2.2, height / 3, col);
	}
	else if (state == State::ATTACKING && look == Look::LEFT && wasCrouching)
	{
		Entity::DrawHitbox(pos.x - (width * 2.1), pos.y - height / 3, width * 2.2, height / 3, col);
	}
	else if (state == State::ATTACKING && look == Look::RIGHT && !wasCrouching)
	{
		Entity::DrawHitbox(pos.x + width, pos.y - height / 2, width * 2.2, height / 3, col);
	}
	else if (state == State::ATTACKING && look == Look::LEFT && !wasCrouching)
	{
		Entity::DrawHitbox(pos.x - (width * 2.1), pos.y - height / 2, width * 2.2, height / 3, col);
	}
	//TODO Change this so that the width and height are appropriate
	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), WINDOW_WIDTH-90, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);

}
void Player::Release()
{
	//if (shieldSFX.stream.processor != NULL)
	//{
	//	UnloadSound(shieldSFX);

	//}
	//if (whipSFX.stream.processor != NULL)
	//{
	//	UnloadSound(whipSFX);

	//}
	//if (moneyBagSFX.stream.processor != NULL)
	//{
	//	UnloadSound(moneyBagSFX);
	//}
	//if (orbSFX.stream.processor != NULL)
	//{
	//	UnloadSound(orbSFX);
	//}
	if (!unloadedSounds)
	{
		// TODO Unload sfx etc
		//UnloadSound(shieldSFX);
		//UnloadSound(whipSFX); // TODO: fix this in case sound is not played 
		//// this happens after the player dies, the sounds will unload somehow?
		//UnloadSound(moneyBagSFX);
		//UnloadSound(orbSFX);
		//unloadedSounds = true;
	}




	//UnloadSound(whipSFX);
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	
	render->Release();


}