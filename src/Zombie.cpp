#include "Zombie.h"
#include "Sprite.h"

Zombie::Zombie(const Point& p, int width, int height, int frame_width, int frame_height) :
	Enemy(p, width, height, frame_width, frame_height)
{
	attack_delay = 0;
	state = ZombieState::ROAMING;

	current_step = 0;
	current_frames = 0;
}
Zombie::~Zombie()
{
}
AppStatus Zombie::Initialise(Look look, const AABB& area)
{
	int i;
	const int n = ZOMBIE_FRAME_SIZE_WIDTH;
	const int h = ZOMBIE_FRAME_SIZE_HEIGHT;

	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_ENEMIES));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for slime sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)ZombieAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)ZombieAnim::IDLE_RIGHT, ZOMBIE_ANIM_DELAY);
	sprite->AddKeyFrame((int)ZombieAnim::IDLE_RIGHT, { 0, 2 * n, n, n });
	sprite->SetAnimationDelay((int)ZombieAnim::IDLE_LEFT, ZOMBIE_ANIM_DELAY);
	sprite->AddKeyFrame((int)ZombieAnim::IDLE_LEFT, { 0, 2 * n, -n, n });

	sprite->SetAnimationDelay((int)ZombieAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 1; i < 2; ++i)
		sprite->AddKeyFrame((int)ZombieAnim::WALKING_RIGHT, { (float)i * n, 2 * n, -n, h });
	sprite->AddKeyFrame((int)ZombieAnim::WALKING_RIGHT, { (float)0 * n, 2 * n, -n, h });

	sprite->SetAnimationDelay((int)ZombieAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 1; i < 2; ++i)
		sprite->AddKeyFrame((int)ZombieAnim::WALKING_LEFT, { (float)i * n, 2 * n, n, h });
	sprite->AddKeyFrame((int)ZombieAnim::WALKING_LEFT, { (float)0 * n, 2 * n, n, h });

	sprite->SetAnimationDelay((int)ZombieAnim::ATTACK_RIGHT, ZOMBIE_ANIM_DELAY);
	sprite->AddKeyFrame((int)ZombieAnim::ATTACK_RIGHT, { 0, 3 * n, n, n });
	sprite->AddKeyFrame((int)ZombieAnim::ATTACK_RIGHT, { n, 3 * n, n, n });
	sprite->SetAnimationDelay((int)ZombieAnim::ATTACK_LEFT, ZOMBIE_ANIM_DELAY);
	sprite->AddKeyFrame((int)ZombieAnim::ATTACK_LEFT, { 0, 3 * n, -n, n });
	sprite->AddKeyFrame((int)ZombieAnim::ATTACK_LEFT, { n, 3 * n, -n, n });

	this->look = look;
	if (look == Look::LEFT)        sprite->SetAnimation((int)ZombieAnim::WALKING_LEFT);
	else if (look == Look::RIGHT) sprite->SetAnimation((int)ZombieAnim::WALKING_RIGHT);

	visibility_area = area;

	InitPattern();

	return AppStatus::OK;
}
void Zombie::InitPattern()
{
	//Multiplying by 3 ensures sufficient time for displaying all 3 frames of the
	//walking animation, resulting in smoother transitions and preventing the animation
	//from appearing rushed or incomplete
	const int n = ZOMBIE_ANIM_DELAY * 3;

	//pattern.push_back({ {0, 0}, 2 * n, (int)ZombieAnim::IDLE_RIGHT });
	//pattern.push_back({ {ZOMBIE_SPEED, 0}, n, (int)ZombieAnim::WALKING_RIGHT });
	//pattern.push_back({ {0, 0}, n, (int)ZombieAnim::IDLE_RIGHT });
	//pattern.push_back({ {ZOMBIE_SPEED, 0}, n, (int)ZombieAnim::WALKING_RIGHT });
	//pattern.push_back({ {0, 0}, n, (int)ZombieAnim::IDLE_RIGHT });

	//pattern.push_back({ {0, 0}, 2 * n, (int)ZombieAnim::IDLE_LEFT });
	//pattern.push_back({ {-ZOMBIE_SPEED, 0}, n, (int)ZombieAnim::WALKING_LEFT });
	//pattern.push_back({ {0, 0}, n, (int)ZombieAnim::IDLE_LEFT });
	//pattern.push_back({ {-ZOMBIE_SPEED, 0}, n, (int)ZombieAnim::WALKING_LEFT });
	//pattern.push_back({ {0, 0}, n, (int)ZombieAnim::IDLE_LEFT });

	//pattern.push_back({ {ZOMBIE_SPEED, 0}, n, (int)ZombieAnim::WALKING_RIGHT });
	pattern.push_back({ {-ZOMBIE_SPEED, 0}, n, (int)ZombieAnim::WALKING_LEFT });

	current_step = 0;
	current_frames = 0;
}
bool Zombie::Update(const AABB& box)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	bool shoot = false;
	int anim_id;

	attack_delay = ZOMBIE_ANIM_DELAY;

	if (look == Look::LEFT)	sprite->SetAnimation((int)ZombieAnim::ATTACK_LEFT);
	else					sprite->SetAnimation((int)ZombieAnim::ATTACK_RIGHT);

	//if (state == ZombieState::ROAMING)
	//{
	//	if (IsVisible(box))
	//	{
	//		state = ZombieState::ATTACK;
	//		//The attack animation consists of 2 frames, with the second one being when
	//		//we throw the shot. Wait for a frame before initiating the attack.
	//		attack_delay = ZOMBIE_ANIM_DELAY;

	//		if (look == Look::LEFT)	sprite->SetAnimation((int)ZombieAnim::ATTACK_LEFT);
	//		else					sprite->SetAnimation((int)ZombieAnim::ATTACK_RIGHT);
	//	}
	//	else
	//	{
	//		pos += pattern[current_step].speed;
	//		current_frames++;

	//		if (current_frames == pattern[current_step].frames)
	//		{
	//			current_step++;
	//			current_step %= pattern.size();
	//			current_frames = 0;

	//			anim_id = pattern[current_step].anim;
	//			sprite->SetAnimation(anim_id);
	//			UpdateLook(anim_id);
	//		}
	//	}
	//}
	//else if (state == ZombieState::ATTACK)
	//{
	//	if (!IsVisible(box))
	//	{
	//		state = ZombieState::ROAMING;

	//		//Continue with the previous animation pattern before initiating the attack
	//		anim_id = pattern[current_step].anim;
	//		sprite->SetAnimation(anim_id);
	//	}
	//	else
	//	{
	//		attack_delay--;
	//		if (attack_delay == 0)
	//		{
	//			shoot = true;

	//			//The attack animation consists of 2 frames. Wait for a complete loop
	//			//before shooting again
	//			attack_delay = 2 * ZOMBIE_ANIM_DELAY;
	//		}
	//	}
	//}
	sprite->Update();

	return shoot;
}
void Zombie::UpdateLook(int anim_id)
{
	ZombieAnim anim = (ZombieAnim)anim_id;
	look = (anim == ZombieAnim::IDLE_LEFT ||
		anim == ZombieAnim::WALKING_LEFT ||
		anim == ZombieAnim::ATTACK_LEFT) ? Look::LEFT : Look::RIGHT;
}
void Zombie::GetShootingPosDir(Point* p, Point* d) const
{
	if (look == Look::LEFT)
	{
		p->x = pos.x + ZOMBIE_SHOT_OFFSET_X_LEFT;
		*d = { -ZOMBIE_SHOT_SPEED, 0 };
	}
	else
	{
		p->x = pos.x + ZOMBIE_SHOT_OFFSET_X_RIGHT;
		*d = { ZOMBIE_SHOT_SPEED, 0 };
	}
	p->y = pos.y + ZOMBIE_SHOT_OFFSET_Y;
}
//
//#include "Enemy.h"
//#include "Sprite.h"
//#include "TileMap.h"
//#include "Globals.h"
//#include <raymath.h>
//
//
//Enemy::Enemy(const Point& p, EnemyState s, EnemyLook view) : Entity(p, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGHT, ENEMY_FRAME_SIZE_WIDTH, ENEMY_FRAME_SIZE_HEIGHT)
//{
//	//Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
//
//	state = s;
//	look = view;
//	map = nullptr;
//	
//}
//Enemy::~Enemy()
//{
//}
//AppStatus Enemy::Initialise()
//{
//	int i;
//	const int n = ENEMY_FRAME_SIZE_WIDTH;
//	const int h = ENEMY_FRAME_SIZE_HEIGHT;
//
//	ResourceManager& data = ResourceManager::Instance();
//	if (data.LoadTexture(Resource::IMG_ENEMIES, "img/enemies.png") != AppStatus::OK)
//	{
//		return AppStatus::ERROR;
//	}
//
//	render = new Sprite(data.GetTexture(Resource::IMG_ENEMIES));
//	if (render == nullptr)
//	{
//		LOG("Failed to allocate memory for enemy sprite");
//		return AppStatus::ERROR;
//	}
//
//	Sprite* sprite = dynamic_cast<Sprite*>(render);
//	sprite->SetNumberAnimations((int)EnemyAnim::NUM_ANIMATIONS);
//
//	// Idle animations
//	sprite->SetAnimationDelay((int)EnemyAnim::IDLE_RIGHT, ANIM_DELAY);
//	sprite->AddKeyFrame((int)EnemyAnim::IDLE_RIGHT, { 0, 2*n, -n, h });
//
//	sprite->SetAnimationDelay((int)EnemyAnim::IDLE_LEFT, ANIM_DELAY);
//	sprite->AddKeyFrame((int)EnemyAnim::IDLE_LEFT, { 0, 2*n, n, h });
//
//	// Walking animations
//	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_RIGHT, ANIM_DELAY);
//	for (i = 1; i < 2; ++i)
//		sprite->AddKeyFrame((int)EnemyAnim::WALKING_RIGHT, { (float)i * n, 2*n, -n, h });
//		sprite->AddKeyFrame((int)EnemyAnim::WALKING_RIGHT, { (float)0 * n, 2*n, -n, h });
//
//	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_LEFT, ANIM_DELAY);
//	for (i = 1; i < 2; ++i)
//		sprite->AddKeyFrame((int)EnemyAnim::WALKING_LEFT, { (float)i * n, 2*n, n, h });
//	sprite->AddKeyFrame((int)EnemyAnim::WALKING_LEFT, { (float)0 * n, 2*n, n, h });
//
//	sprite->SetAnimation((int)EnemyAnim::IDLE_LEFT);
//
//
//	return AppStatus::OK;
//}
//
//int Enemy::GetXPos()
//{
//	return pos.x;
//}
//int Enemy::GetYPos()
//{
//	return pos.y;
//}
//void Enemy::SetTileMap(TileMap* tilemap)
//{
//	map = tilemap;
//}
//bool Enemy::IsLookingRight() const
//{
//	return look == EnemyLook::RIGHT;
//}
//bool Enemy::IsLookingLeft() const
//{
//	return look == EnemyLook::LEFT;
//}
//void Enemy::SetAnimation(int id)
//{
//	Sprite* sprite = dynamic_cast<Sprite*>(render);
//	sprite->SetAnimation(id);
//}
//EnemyAnim Enemy::GetAnimation()
//{
//	Sprite* sprite = dynamic_cast<Sprite*>(render);
//	return (EnemyAnim)sprite->GetAnimation();
//}
//void Enemy::Stop()
//{
//	dir = { 0,0 };
//	state = EnemyState::IDLE;
//	if (IsLookingRight())
//	{
//		SetAnimation((int)EnemyAnim::IDLE_RIGHT);
//	}
//	else
//	{
//		SetAnimation((int)EnemyAnim::IDLE_LEFT);
//	}
//}
//void Enemy::StartWalkingLeft()
//{
//	state = EnemyState::WALKING;
//	look = EnemyLook::LEFT;
//	SetAnimation((int)EnemyAnim::WALKING_LEFT);
//}
//void Enemy::StartWalkingRight()
//{
//	state = EnemyState::WALKING;
//	look = EnemyLook::RIGHT;
//	SetAnimation((int)EnemyAnim::WALKING_RIGHT);
//}
//void Enemy::ChangeAnimRight()
//{
//	look = EnemyLook::RIGHT;
//	switch (state)
//	{
//		case EnemyState::IDLE:	 
//			SetAnimation((int)EnemyAnim::IDLE_RIGHT);
//			break; 
//		case EnemyState::WALKING:
//			SetAnimation((int)EnemyAnim::WALKING_RIGHT);
//			break;
//		
//	}
//}
//void Enemy::ChangeAnimLeft()
//{
//	look = EnemyLook::LEFT;
//	switch (state)
//	{
//		case EnemyState::IDLE:
//			SetAnimation((int)EnemyAnim::IDLE_LEFT);
//			break;
//		case EnemyState::WALKING:
//			SetAnimation((int)EnemyAnim::WALKING_LEFT);
//			break;
//	}
//}
//void Enemy::Update()
//{
//	
//	MoveX();
//	/*MoveY();*/
//	
//
//	Sprite* sprite = dynamic_cast<Sprite*>(render);
//	sprite->Update();
//}
//void Enemy::MoveX()
//{
//	AABB box;
//	int prev_x = pos.x;
//
//	
//	if (IsLookingRight())
//	{
//		pos.x += ENEMY_SPEED;
//		if (state == EnemyState::IDLE)
//		{
//			StartWalkingRight();
//			
//		}
//		else
//		{
//			if (IsLookingLeft()) ChangeAnimLeft();
//		}
//
//		box = GetHitbox();
//		if (map->TestCollisionWallRight(box))
//		{
//			pos.x = prev_x;
//			if (state == EnemyState::WALKING) Stop();
//		}
//	}
//	else if (IsLookingLeft())
//	{
//		pos.x +=	-ENEMY_SPEED;
//		if (state == EnemyState::IDLE)
//		{
//			StartWalkingLeft();
//			
//		}
//		else
//		{
//			if (IsLookingRight()) ChangeAnimRight();
//		}
//
//		/*box = GetHitbox();
//		if (map->TestCollisionWallLeft(box))
//		{
//			pos.x = prev_x;
//			if (state == EnemyState::WALKING) Stop();
//		}*/
//	}
//	
//}
////void Enemy::MoveY()
////{
////	AABB box;
////
////	pos.y += ENEMY_SPEED;
////	box = GetHitbox();
////	if (map->TestCollisionGround(box, &pos.y))
////	{
////		if (state == State::FALLING) Stop();
////
////		if (IsKeyDown(KEY_UP))
////		{
////			box = GetHitbox();
////			if (map->TestOnLadder(box, &pos.x))
////				StartClimbingUp();
////			else
////			{
////				StartJumping();
////			}
////		}
////		else if (IsKeyDown(KEY_DOWN))
////		{
////			//To climb up the ladder, we need to check the control point (x, y)
////			//To climb down the ladder, we need to check pixel below (x, y+1) instead
////			box = GetHitbox();
////			box.pos.y++;
////			if (map->TestOnLadderTop(box, &pos.x))
////			{
////				StartClimbingDown();
////				pos.y += PLAYER_LADDER_SPEED;
////				pos.x -= PLAYER_LADDER_SPEED;
////			}
////			else
////			{
////				StartCrouching();
////			}
////
////		}
////		else if (IsKeyPressed(KEY_SPACE))
////		{
////			Attack();
////		}
////		else if (IsKeyPressed(KEY_L))
////		{
////			Death();
////		}
////	}
////}
//
//void Enemy::DrawDebug(const Color& col) const
//{	
//	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
//	//TODO Change this so that the width and height are appropriate
//	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), WINDOW_WIDTH-90, 50, 8, LIGHTGRAY);
//	DrawPixel(pos.x, pos.y, WHITE);
//}
//void Enemy::Release()
//{
//	ResourceManager& data = ResourceManager::Instance();
//	data.ReleaseTexture(Resource::IMG_ENEMIES);
//
//	render->Release();
//}