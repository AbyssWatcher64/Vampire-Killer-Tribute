#include "Cutscene.h"
#include <stdio.h>
#include "Globals.h"

Cutscene::Cutscene()
{
	player = nullptr;
	timer = 0;
	img_intro = nullptr;
}
Cutscene::~Cutscene()
{
	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
}
AppStatus Cutscene::Init()
{
	
	player = new Player({ 0,0 }, State::IDLE, Look::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}
	player->InitPattern();
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_INTRO, "img/misc.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_intro = data.GetTexture(Resource::IMG_INTRO);
	
	player->SetPos(Point(WINDOW_WIDTH - PLAYER_PHYSICAL_WIDTH, 199));
	return AppStatus::OK;
}
AppStatus Cutscene::LoadResources()
{
	
}
void Cutscene::UpdateCutscene()
{
	
	Point p1, p2;
	AABB hitbox;
	
	player->UpdatePattern();
	hitbox = player->GetHitbox();
	
}
void Cutscene::Render()
{
	DrawTexturePro(*img_intro, { 8,246,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0 }, 0, WHITE);
	player->Draw();
}
void Cutscene::Release()
{
	player->Release();
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_INTRO);
}