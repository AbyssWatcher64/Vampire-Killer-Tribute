
#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Player::Player(const Point& p, int width, int height, State s, Look view) : Entity(p, width, height)
{
	state = s;
	look = view;
}
Player::~Player()
{
}
AppStatus Player::Initialise()
{
	int i;
	const int n = PLAYER_FRAME_SIZE;

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
	
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT, { 0, 0, n, n*2 });
	
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT, { 0, 0, -n, n*2 });

	// The reason this uses a for and a hard-coded line is because otherwise the animation
	// will start as Simon walking
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 1; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { (float)i*n, 0, n, n*2 });
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { (float)0*n, 0, n, n*2 });
		//sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { (float)i*n, 3*n, n/2, n });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 1; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { (float)i*n, 0, -n, n*2 });
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { (float)0*n, 0, -n, n*2 });
		//sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { (float)i*n, 3*n, -n/2, n });
	
	// Death animation
	sprite->SetAnimationDelay((int)PlayerAnim::DYING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
	{
		sprite->AddKeyFrame((int)PlayerAnim::DYING_RIGHT, { ((float)i + 6) * n, 0, n, n * 2 });
	}
	sprite->AddKeyFrame((int)PlayerAnim::DYING_RIGHT, { ((float)i + 6) * n, 0, n * 2, n * 2 });

	sprite->SetAnimationDelay((int)PlayerAnim::DYING_LEFT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
	{
		sprite->AddKeyFrame((int)PlayerAnim::DYING_LEFT, { ((float)i + 6)* n, 0, -n, n * 2 });
	}
	// TOASK: Same problem here, animation goes rightway
	sprite->AddKeyFrame((int)PlayerAnim::DYING_LEFT, { ((float)i + 6)* n, 0, -n * 2 , n * 2 });


	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_RIGHT_WHIP, ANIM_DELAY);
	//for (i = 0; i < 3; ++i)
	//	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_RIGHT_WHIP, { (float)i * n, n*6, n * 2 , n * 2 });
	//TOASK: How can we do a pivot point through the PlayerAnim
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_RIGHT_WHIP, { (float)0 * n, n * 6, n * 2 , n * 2 });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_RIGHT_WHIP, { (float)2 * n, n * 6, n * 2 , n * 2 });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_RIGHT_WHIP, { (float)4 * n, n * 6, n * 4 , n * 2 });

	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_LEFT_WHIP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_LEFT_WHIP, { (float)0 * n, n * 6, -(n * 2) , n * 2 });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_LEFT_WHIP, { (float)2 * n, n * 6, -(n * 2) , n * 2 });
	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_LEFT_WHIP, { (float)4 * n, n * 6, -(n * 4) , n * 2 });
	//for (i = 0; i < 3; ++i)
	//	sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_LEFT_WHIP, { (float)i * n, n * 6, -(n * 2) , n * 2 });

	sprite->SetAnimation((int)PlayerAnim::IDLE_RIGHT);

	return AppStatus::OK;	
}
void Player::SetState(State s)
{
	state = s;
}
State Player::GetState() const
{
	return state;
}
void Player::SetLook(Look view)
{
	look = view;
}
bool Player::IsLookingRight() const
{
	return look == Look::RIGHT;
}
void Player::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
void Player::Stop()
{
	SetDirection({ 0,0 });
	state = State::IDLE;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::IDLE_RIGHT);
	else					SetAnimation((int)PlayerAnim::IDLE_LEFT);
}
void Player::StartWalkingLeft()
{
	SetDirection({ -PLAYER_SPEED,0 });
	state = State::WALKING;
	look = Look::LEFT;
	SetAnimation((int)PlayerAnim::WALKING_LEFT);
}
void Player::StartWalkingRight()
{
	SetDirection({ PLAYER_SPEED,0 });
	state = State::WALKING;
	look = Look::RIGHT;
	SetAnimation((int)PlayerAnim::WALKING_RIGHT);
}
void Player::Attack()
{
	state = State::ATTACKING;
	if (look == Look::RIGHT)
	{
		SetAnimation((int)PlayerAnim::ATTACKING_RIGHT_WHIP);
	}
	else if (look == Look::LEFT)
	{
		SetAnimation((int)PlayerAnim::ATTACKING_LEFT_WHIP);
	}
	state = State::IDLE;
}
//TODO: Create death animation
void Player::Death()
{
	state = State::DEAD;
	if (look == Look::RIGHT)
	{
		SetAnimation((int)PlayerAnim::DYING_RIGHT);
	}
	else if (look == Look::LEFT)
	{
		SetAnimation((int)PlayerAnim::DYING_LEFT);
	}
}
void Player::Update()
{
	Entity::Update();
	
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Player::DrawDebug(const Color& col) const
{	
	int x, y;
	x = pos.x + PLAYER_FRAME_SIZE - PLAYER_PHYSICAL_WIDTH;
	//x = pos.x + PLAYER_FRAME_SIZE/2 - PLAYER_PHYSICAL_WIDTH/2;
	//y = pos.y + PLAYER_FRAME_SIZE - PLAYER_PHYSICAL_HEIGHT;
	y = pos.y;

	Entity::DrawDebug(x, y, width, height, col);
}
void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}