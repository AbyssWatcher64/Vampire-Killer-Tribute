
#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Player::Player(const Point& p, State s, Look view) :
	//Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE_WIDTH, PLAYER_FRAME_SIZE_HEIGHT)
{
	state = s;
	look = view;
	jump_delay = PLAYER_JUMP_DELAY;
	map = nullptr;
	isHoldingShield = false;
}
Player::~Player()
{
}
AppStatus Player::Initialise()
{
	int i;
	//const int n = PLAYER_FRAME_SIZE;
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
	
	//sprite->SetAnimationDelay((int)PlayerAnim::IDLE_RIGHT, ANIM_DELAY);
	//sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT, { 0, 0, n, n });
	//sprite->SetAnimationDelay((int)PlayerAnim::IDLE_LEFT, ANIM_DELAY);
	//sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT, { 0, 0, -n, n });

	//sprite->SetAnimationDelay((int)PlayerAnim::WALKING_RIGHT, ANIM_DELAY);
	//for (i = 0; i < 8; ++i)
	//	sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { (float)i*n, 4*n, n, n });
	//sprite->SetAnimationDelay((int)PlayerAnim::WALKING_LEFT, ANIM_DELAY);
	//for (i = 0; i < 8; ++i)
	//	sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { (float)i*n, 4*n, -n, n });



	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING, ANIM_LADDER_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CLIMBING, { (float)i * n, 6 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_PRE_TOP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_PRE_TOP, { 4 * n, 6 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_TOP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_TOP, { 5 * n, 6 * n, n, n });

	//TODO: Change Climbing animations
	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_SHIELD, ANIM_LADDER_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_SHIELD, { (float)i * n, 6 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_PRE_TOP_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_PRE_TOP_SHIELD, { 4 * n, 6 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_TOP_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_TOP_SHIELD, { 5 * n, 6 * n, n, n });
	



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
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT, { 3 * n, h, n, h });
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT, { 3 * n, h, n, h });
	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT, { 3 * n, h, -n, h });
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT, { 3 * n, h, -n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_RIGHT, { 3 * n, h, n, h });
	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_LEFT, { 3 * n, h, -n, h });
	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_RIGHT, { 3 * n, h, n, h });
	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_LEFT, { 3 * n, h, -n, h });

	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_RIGHT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT_SHIELD, { 3 * n, h, n, h });
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT_SHIELD, { 3 * n, h, n, h });
	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_LEFT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT_SHIELD, { 3 * n, h, -n, h });
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
	for (i = 0; i < 2; ++i)
	{
		sprite->AddKeyFrame((int)PlayerAnim::DYING_RIGHT, { ((float)i + 6) * n, 0, n, h });
	}
	sprite->AddKeyFrame((int)PlayerAnim::DYING_RIGHT, { ((float)i + 6) * n, 0, n * 2, h });

	sprite->SetAnimationDelay((int)PlayerAnim::DYING_LEFT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
	{
		sprite->AddKeyFrame((int)PlayerAnim::DYING_LEFT, { ((float)i + 6) * n, 0, -n, h });
	}
	// TOASK: Same problem here, animation goes rightway
	sprite->AddKeyFrame((int)PlayerAnim::DYING_LEFT, { ((float)i + 6) * n, 0, -n * 2 , h });

	// Walking towards front animation
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_INTO, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::WALKING_INTO, { 10 * n, 0, n, h });


	// WHIP Attacking animations
	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_LEFT_WHIP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_LEFT_WHIP, { (float)0 * n, n * 6, -(n * 2) , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_LEFT_WHIP, { (float)2 * n, n * 6, -(n * 2) , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_LEFT_WHIP, { (float)4 * n, n * 6, -(n * 4) , h });

	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_CROUCHING_LEFT_WHIP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_CROUCHING_LEFT_WHIP, { (float)0 * n, n * 8, -(n * 2) , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_CROUCHING_LEFT_WHIP, { (float)2 * n, n * 8, -(n * 2) , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_CROUCHING_LEFT_WHIP, { (float)4 * n, n * 8, -(n * 4) , h });

	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_UPSTAIRS_LEFT_WHIP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_UPSTAIRS_LEFT_WHIP, { (float)0 * n, n * 10, -(n * 2) , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_UPSTAIRS_LEFT_WHIP, { (float)2 * n, n * 10, -(n * 2) , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_UPSTAIRS_LEFT_WHIP, { (float)4 * n, n * 10, -(n * 4) , h });

	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_DOWNSTAIRS_LEFT_WHIP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_DOWNSTAIRS_LEFT_WHIP, { (float)0 * n, n * 12, -(n * 2) , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_DOWNSTAIRS_LEFT_WHIP, { (float)2 * n, n * 12, -(n * 2) , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_DOWNSTAIRS_LEFT_WHIP, { (float)4 * n, n * 12, -(n * 4) , h });



	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_RIGHT_WHIP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_RIGHT_WHIP, { (float)0 * n, n * 6, n * 2 , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_RIGHT_WHIP, { (float)2 * n, n * 6, n * 2 , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_RIGHT_WHIP, { (float)4 * n, n * 6, n * 4 , h });

	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_CROUCHING_RIGHT_WHIP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_CROUCHING_RIGHT_WHIP, { (float)0 * n, n * 8, -(n * 2) , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_CROUCHING_RIGHT_WHIP, { (float)2 * n, n * 8, -(n * 2) , h });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_CROUCHING_RIGHT_WHIP, { (float)4 * n, n * 8, -(n * 4) , h });

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


	return AppStatus::OK;
}
bool Player::GetIsHoldingShield() const
{
	return Player::isHoldingShield;
}

void Player::SetWeapon(Weapon w)
{
	weapon = w;
}
Weapon Player::EquipWhip()
{
	return weapon = Weapon::WHIP;
}
Weapon Player::EquipMorningStar()
{
	return weapon = Weapon::MORNINGSTAR;
}
Weapon Player::EquipKnife()
{
	return weapon = Weapon::KNIFE;
}
void Player::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
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
		if (GetIsHoldingShield() == true)
			SetAnimation((int)PlayerAnim::CROUCHING_LEFT_SHIELD);
		else
			SetAnimation((int)PlayerAnim::CROUCHING_LEFT);
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
		
	else					SetAnimation((int)PlayerAnim::FALLING_LEFT);
}
void Player::StartJumping()
{
	dir.y = -PLAYER_JUMP_FORCE;
	state = State::JUMPING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
	else					SetAnimation((int)PlayerAnim::JUMPING_LEFT);
	jump_delay = PLAYER_JUMP_DELAY;
}
void Player::StartClimbingUp()
{
	state = State::CLIMBING;
	SetAnimation((int)PlayerAnim::CLIMBING);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}
void Player::StartClimbingDown()
{
	state = State::CLIMBING;
	SetAnimation((int)PlayerAnim::CLIMBING_TOP);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}
void Player::Attack()
{
	state = State::ATTACKING;
	if (look == Look::RIGHT)
	{
		SetAnimation((int)PlayerAnim::ATTACKING_RIGHT_WHIP);
		Sprite* sprite = dynamic_cast<Sprite*>(render);
		sprite->SetPlayOnceMode();
		//state = State::IDLE;
	}
	else if (look == Look::LEFT)
	{
		SetAnimation((int)PlayerAnim::ATTACKING_LEFT_WHIP);
		Sprite* sprite = dynamic_cast<Sprite*>(render);
		sprite->SetPlayOnceMode();
	}
	state = State::IDLE;
}
void Player::Death()
{
	state = State::DEAD;
	if (look == Look::RIGHT)
	{
		SetAnimation((int)PlayerAnim::DYING_RIGHT);
		WaitTime(2);
	}
	else if (look == Look::LEFT)
	{
		SetAnimation((int)PlayerAnim::DYING_LEFT);
		WaitTime(2);
	}
}
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
			//if (GetIsHoldingShield() == true)
			//	SetAnimation((int)PlayerAnim::FALLING_RIGHT_SHIELD);
			//else
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
		case State::FALLING: SetAnimation((int)PlayerAnim::FALLING_LEFT); break;
	}
}
void Player::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	MoveX();
	MoveY();

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Player::MoveX()
{
	AABB box;
	int prev_x = pos.x;

	//We can only go up and down while climbing
	if (state == State::CLIMBING)	return;

	if (IsKeyPressed(KEY_F5))
	{
		isHoldingShield = !isHoldingShield;
		Stop();
	}

	if (IsKeyDown(KEY_DOWN))
	{
		if (state == State::IDLE) StartCrouching();
		else if (state == State::WALKING) StartCrouching();
	}
	else if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT))
	{
		pos.x += -PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingLeft();
		else if (state == State::CROUCHING) StartWalkingLeft();
		else
		{
			if (IsLookingRight()) ChangeAnimLeft();
		}

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
	}
	else if (IsKeyDown(KEY_RIGHT))
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
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
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

	if (state == State::JUMPING)
	{
		LogicJumping();
	}
	else if (state == State::CLIMBING)
	{
		LogicClimbing();
	}
	else //idle, walking, falling
	{
		pos.y += PLAYER_SPEED;
		box = GetHitbox();
		if (map->TestCollisionGround(box, &pos.y))
		{
			if (state == State::FALLING) Stop();

			if (IsKeyDown(KEY_UP))
			{
				box = GetHitbox();
				if (map->TestOnLadder(box, &pos.x))
					StartClimbingUp();
				else
				{
					StartJumping();
				}
			}
			else if (IsKeyDown(KEY_DOWN))
			{
				//To climb up the ladder, we need to check the control point (x, y)
				//To climb down the ladder, we need to check pixel below (x, y+1) instead
				box = GetHitbox();
				box.pos.y++;
				if (map->TestOnLadderTop(box, &pos.x))
				{
					StartClimbingDown();
					pos.y += PLAYER_LADDER_SPEED;
				}
					
			}
			else if (IsKeyPressed(KEY_SPACE))
			{
				Attack();
			}
			else if (IsKeyPressed(KEY_L))
			{
				Death();
			}
		}
		else
		{
			if (state != State::FALLING) StartFalling();
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
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
				else					SetAnimation((int)PlayerAnim::JUMPING_LEFT);
			}
			else if (IsLevitating())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::LEVITATING_RIGHT);
				else					SetAnimation((int)PlayerAnim::LEVITATING_LEFT);
			}
			else if (IsDescending())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::FALLING_RIGHT);
				else					SetAnimation((int)PlayerAnim::FALLING_LEFT);
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
	AABB box;
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	int tmp;

	if (IsKeyDown(KEY_UP))
	{
		pos.y -= PLAYER_LADDER_SPEED;
		sprite->NextFrame();
	}
	else if (IsKeyDown(KEY_DOWN))
	{
		pos.y += PLAYER_LADDER_SPEED;
		sprite->PrevFrame();
	}

	//It is important to first check LadderTop due to its condition as a collision ground.
	//By doing so, we ensure that we don't stop climbing down immediately after starting the descent.
	box = GetHitbox();
	if (map->TestOnLadderTop(box, &tmp))
	{
		if (IsInSecondHalfTile())		SetAnimation((int)PlayerAnim::CLIMBING_PRE_TOP);
		else if (IsInFirstHalfTile())	SetAnimation((int)PlayerAnim::CLIMBING_TOP);
		else					LOG("Internal error, tile should be a LADDER_TOP, coord: (%d,%d)", box.pos.x, box.pos.y);
	}
	else if (map->TestCollisionGround(box, &pos.y))
	{
		//Case leaving the ladder descending.
		Stop();
		sprite->SetAutomaticMode();
	}
	else if (!map->TestOnLadder(box, &tmp))
	{
		//Case leaving the ladder ascending.
		//If we are not in a LadderTop, colliding ground or in the Ladder it means we are leaving
		//ther ladder ascending.
		Stop();
		sprite->SetAutomaticMode();
	}
	else
	{
		if (GetAnimation() != PlayerAnim::CLIMBING)	SetAnimation((int)PlayerAnim::CLIMBING);
	}
}
void Player::DrawDebug(const Color& col) const
{	
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
	
	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18*16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}