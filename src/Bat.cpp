#include "Bat.h"
#include "Sprite.h"

Bat::Bat(const Point& p, int width, int height, int frame_width, int frame_height) :
	Enemy(p, width, height, frame_width, frame_height)
{
	attack_delay = 0;
	state = BatState::ROAMING;

	current_step = 0;
	current_frames = 0;
	type = EnemyType::BAT;
}
Bat::~Bat()
{
}
AppStatus Bat::Initialise(Look look, const AABB& area)
{
	int i;
	const int n = BAT_FRAME_SIZE_WIDTH;
	const int h = BAT_FRAME_SIZE_HEIGHT;

	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_ENEMIES));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for bat sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)BatAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)BatAnim::FLYING_RIGHT, ANIM_DELAY + 10 );
	/*for (i = 1; i < 2; ++i)
		sprite->AddKeyFrame((int)BatAnim::FLYING_RIGHT, { (float)i * n, 7 * n, -n, h });*/
	sprite->AddKeyFrame((int)BatAnim::FLYING_RIGHT, { (float)2 * n, 7 * n, -n, h });
	sprite->AddKeyFrame((int)BatAnim::FLYING_RIGHT, { (float)3 * n, 7 * n, -n, h });
	sprite->AddKeyFrame((int)BatAnim::FLYING_RIGHT, { (float)2 * n, 7 * n, -n, h });
	sprite->AddKeyFrame((int)BatAnim::FLYING_RIGHT, { (float)1 * n, 7 * n, -n, h });

	sprite->SetAnimationDelay((int)BatAnim::FLYING_LEFT, ANIM_DELAY + 10);
	sprite->AddKeyFrame((int)BatAnim::FLYING_LEFT, { (float)2 * n, 7 * n, n, h });
	sprite->AddKeyFrame((int)BatAnim::FLYING_LEFT, { (float)3 * n, 7 * n, n, h });
	sprite->AddKeyFrame((int)BatAnim::FLYING_LEFT, { (float)2 * n, 7 * n, n, h });
	sprite->AddKeyFrame((int)BatAnim::FLYING_LEFT, { (float)1 * n, 7 * n, n, h });
	/*for (i = 1; i < 2; ++i)
		sprite->AddKeyFrame((int)BatAnim::FLYING_LEFT, { (float)i * n, 7 * n, n, h });*/


	this->look = look;
	if (look == Look::LEFT)        sprite->SetAnimation((int)BatAnim::FLYING_LEFT);
	else if (look == Look::RIGHT) sprite->SetAnimation((int)BatAnim::FLYING_RIGHT);

	visibility_area = area;

	InitPattern();

	return AppStatus::OK;
}
void Bat::InitPattern()
{
	//Multiplying by 3 ensures sufficient time for displaying all 3 frames of the
	//walking animation, resulting in smoother transitions and preventing the animation
	//from appearing rushed or incomplete
	const int n = BAT_ANIM_DELAY;

	if (look == Look::LEFT)
	{
		pattern.push_back({ {-BAT_SPEED, 1},  n, (int)BatAnim::FLYING_LEFT });
		pattern.push_back({ {-BAT_SPEED, 1},  n, (int)BatAnim::FLYING_LEFT });

		pattern.push_back({ {-BAT_SPEED, 0},  n, (int)BatAnim::FLYING_LEFT });
		pattern.push_back({ {-BAT_SPEED, 0},  n, (int)BatAnim::FLYING_LEFT });

		pattern.push_back({ {-BAT_SPEED, -1}, n, (int)BatAnim::FLYING_LEFT });
		pattern.push_back({ {-BAT_SPEED, -1}, n, (int)BatAnim::FLYING_LEFT });

		pattern.push_back({ {-BAT_SPEED, 0},  n, (int)BatAnim::FLYING_LEFT });
		pattern.push_back({ {-BAT_SPEED, 0},  n, (int)BatAnim::FLYING_LEFT });

		pattern.push_back({ {-BAT_SPEED, 1},  n, (int)BatAnim::FLYING_LEFT });
		pattern.push_back({ {-BAT_SPEED, 1},  n, (int)BatAnim::FLYING_LEFT });

		pattern.push_back({ {-BAT_SPEED, 0},  n, (int)BatAnim::FLYING_LEFT });
		pattern.push_back({ {-BAT_SPEED, 0},  n, (int)BatAnim::FLYING_LEFT });

		pattern.push_back({ {-BAT_SPEED, -1}, n, (int)BatAnim::FLYING_LEFT });
		pattern.push_back({ {-BAT_SPEED, -1}, n, (int)BatAnim::FLYING_LEFT });

		pattern.push_back({ {-BAT_SPEED, 0},  n, (int)BatAnim::FLYING_LEFT });
		pattern.push_back({ {-BAT_SPEED, 0},  n, (int)BatAnim::FLYING_LEFT });
	}
	else
	{
		pattern.push_back({ {BAT_SPEED, 1},  n, (int)BatAnim::FLYING_RIGHT });
		pattern.push_back({ {BAT_SPEED, 1},  n, (int)BatAnim::FLYING_RIGHT });
		pattern.push_back({ {BAT_SPEED, 0},  n, (int)BatAnim::FLYING_RIGHT });
		pattern.push_back({ {BAT_SPEED, 0},  n, (int)BatAnim::FLYING_RIGHT });
		pattern.push_back({ {BAT_SPEED, -1}, n,  (int)BatAnim::FLYING_RIGHT });
		pattern.push_back({ {BAT_SPEED, -1}, n,  (int)BatAnim::FLYING_RIGHT });
		pattern.push_back({ {BAT_SPEED, 0},  n, (int)BatAnim::FLYING_RIGHT });
		pattern.push_back({ {BAT_SPEED, 0},  n, (int)BatAnim::FLYING_RIGHT });
		pattern.push_back({ {BAT_SPEED, 1},  n, (int)BatAnim::FLYING_RIGHT });
		pattern.push_back({ {BAT_SPEED, 1},  n, (int)BatAnim::FLYING_RIGHT });
		pattern.push_back({ {BAT_SPEED, 0},  n, (int)BatAnim::FLYING_RIGHT });
		pattern.push_back({ {BAT_SPEED, 0},  n, (int)BatAnim::FLYING_RIGHT });
		pattern.push_back({ {BAT_SPEED, -1}, n,  (int)BatAnim::FLYING_RIGHT });
		pattern.push_back({ {BAT_SPEED, -1}, n,  (int)BatAnim::FLYING_RIGHT });
		pattern.push_back({ {BAT_SPEED, 0},  n, (int)BatAnim::FLYING_RIGHT });
		pattern.push_back({ {BAT_SPEED, 0},  n, (int)BatAnim::FLYING_RIGHT });
	}

	current_step = 0;
	current_frames = 0;
}
bool Bat::Update(const AABB& box)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	bool shoot = false;
	int anim_id;

	
	current_frames++;
	pos += pattern[current_step].speed;

	if (current_frames == pattern[current_step].frames)
	{
		current_step++;
		current_step %= pattern.size();
		current_frames = 0;

		anim_id = pattern[current_step].anim;
		/*sprite->SetAnimation(anim_id);*/
		UpdateLook(anim_id);
	}

	//if (pos.x == -10)
	//	alive = false;
	//pos.x = 299;
//delete this;	

	//else if (pos.x == 300)
	//	alive = false
		//pos.x = -9;
		

	sprite->Update();

	return shoot;
}

void Bat::UpdateLook(int anim_id)
{
	BatAnim anim = (BatAnim)anim_id;
	look =	anim == BatAnim::FLYING_LEFT ? Look::LEFT : Look::RIGHT;
}
void Bat::GetShootingPosDir(Point* p, Point* d) const
{
}
