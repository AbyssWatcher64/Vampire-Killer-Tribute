#include "Cutscene.h"
#include <stdio.h>
#include "Globals.h"

Cutscene::Cutscene()
{
	player = nullptr;
	timer = 0;
	img_intro = nullptr;
	cloud = nullptr;
	cutsceneFinished = false;
	posx = WINDOW_WIDTH - 70;
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
	timer = 0;
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
	img_intro = data.GetTexture(Resource::IMG_CLOUD);
	if (data.LoadTexture(Resource::IMG_CLOUD, "img/misc.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	cloud = data.GetTexture(Resource::IMG_CLOUD);
	player->SetPos(Point(WINDOW_WIDTH - PLAYER_PHYSICAL_WIDTH, 199));
	return AppStatus::OK;
}
void Cutscene::UpdateCutscene()
{
	timer++;
	
	Point p1, p2;
	AABB hitbox;
	
	player->UpdatePattern();
	hitbox = player->GetHitbox();
	if ((timer % 10) == 0)
	{
		posx--;
	}
	if (timer==440 || IsKeyPressed(KEY_SPACE))
	{
		timer = 0;
		cutsceneFinished = true;
		
	}
	
}
void Cutscene::Render()
{
	DrawTexturePro(*img_intro, { 8,246,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0 }, 0, WHITE);
	DrawTexturePro(*cloud, { 272,240,59,16 }, { posx,64,59,16 }, { 0,0 }, 0, WHITE);
	player->Draw();
}
void Cutscene::Release()
{
	player->Release();
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_INTRO);
	data.ReleaseTexture(Resource::IMG_CLOUD);
}