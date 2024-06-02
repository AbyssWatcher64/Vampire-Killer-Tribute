#pragma once

//Displaying scale factor
#define GAME_SCALE_FACTOR	1.0f

//Function return values
enum class AppStatus { OK = 0, ERROR, QUIT };

//Game states - declared in Game.h
enum class GameState { INITIAL_SCREEN, DESCRIPTION_SCREEN, MAIN_MENU,INTRO, PLAYING, SETTINGS, CREDITS, GAME_OVER, ENDING };
//Main return values
#define EXIT_SUCCESS	0
#define EXIT_FAILURE	1

//Log messages
#define LOG(text, ...) log(__FILE__, __LINE__, text, __VA_ARGS__);
void log(const char file[], int line, const char* text, ...);

//Window size
#define WINDOW_WIDTH    256
//#define WINDOW_HEIGHT   208
#define WINDOW_HEIGHT   216
#define WINDOW_HEIGHT_OLD_MARGIN   208

//Game constants
#define MARGIN_GUI_Y	40
#define TILE_SIZE		16
#define LEVEL_WIDTH		16
#define LEVEL_HEIGHT	11

//Entities animation delay
//#define ANIM_DELAY		20 // for testing and checking anims
//#define ANIM_DELAY		8
#define ANIM_DELAY		4
#define ANIM_DELAY_FIRE	8
//#define ANIM_DELAY_WHIP	4

