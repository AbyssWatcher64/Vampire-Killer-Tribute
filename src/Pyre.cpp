#include "Pyre.h"
#include "Sprite.h"

Pyre::Pyre(const Point& p, int width, int height, int frame_width, int frame_height) :
	Enemy(p, width, height, frame_width, frame_height)
{
	attack_delay = 0;
	state = PyreState::STATIC;

	current_step = 0;
	current_frames = 0;
	type = EnemyType::PYRE;
}
Pyre::~Pyre()
{
}
AppStatus Pyre::Initialise(Look look, const AABB& area)
{
	const int n = PYRE_PHYSICAL_SIZE * 2;
	const int h = PYRE_PHYSICAL_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_ITEMS));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for pyre sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)PyreAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)PyreAnim::STATIC, ANIM_DELAY + 8);
	/*for (i = 1; i < 2; ++i)
		sprite->AddKeyFrame((int)BatAnim::FLYING_RIGHT, { (float)i * n, 7 * n, -n, h });*/
	sprite->AddKeyFrame((int)PyreAnim::STATIC, { (float)0 * n, 4 * h, n, h });
	sprite->AddKeyFrame((int)PyreAnim::STATIC, { (float)1 * n, 4 * h, n, h });

	sprite->SetAnimationDelay((int)PyreAnim::DYING, ANIM_DELAY);
	sprite->AddKeyFrame((int)PyreAnim::DYING, { (float)2 * n, 7 * n, n, h });
	sprite->AddKeyFrame((int)PyreAnim::DYING, { (float)3 * n, 7 * n, n, h });
	sprite->AddKeyFrame((int)PyreAnim::DYING, { (float)2 * n, 7 * n, n, h });

	sprite->SetAnimationDelay((int)PyreAnim::DYING_ITEM, ANIM_DELAY);
	sprite->AddKeyFrame((int)PyreAnim::DYING_ITEM, { (float)2 * n, 7 * n, n, h });
	sprite->AddKeyFrame((int)PyreAnim::DYING_ITEM, { (float)3 * n, 7 * n, n, h });
	sprite->AddKeyFrame((int)PyreAnim::DYING_ITEM, { (float)2 * n, 7 * n, n, h });

	this->look = look;
	sprite->SetAnimation((int)PyreAnim::STATIC);

	visibility_area = area;

	InitPattern();

	return AppStatus::OK;
}
void Pyre::InitPattern()
{
	//Multiplying by 3 ensures sufficient time for displaying all 3 frames of the
	//walking animation, resulting in smoother transitions and preventing the animation
	//from appearing rushed or incomplete
	const int n = PYRE_ANIM_DELAY;

	pattern.push_back({ {PYRE_SPEED, 1},  n, (int)PyreAnim::STATIC });
	pattern.push_back({ {PYRE_SPEED, 1},  n, (int)PyreAnim::STATIC });
	

	current_step = 0;
	current_frames = 0;
}
bool Pyre::Update(const AABB& box)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	bool shoot = false;
	int anim_id;


	current_frames++;
	//pos += pattern[current_step].speed;

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

void Pyre::UpdateLook(int anim_id)
{
	PyreAnim anim = (PyreAnim)anim_id;
	look = anim == PyreAnim::STATIC ? Look::LEFT : Look::RIGHT;
}
void Pyre::GetShootingPosDir(Point* p, Point* d) const
{
}
