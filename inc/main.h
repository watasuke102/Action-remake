#pragma once

////////////////////////
//OpenSiv3D-0.4.2
#include <iostream>
#include <Siv3D.hpp>


///////////////////////
//constexpr
constexpr int WINDOW_X = 1920;
constexpr int WINDOW_Y = 1080;
constexpr Size WINDOW_SIZE(WINDOW_X,WINDOW_Y);

constexpr int PLAYER_SPEED     =  6;
constexpr int PLAYER_HIGHSPEED = 12;
constexpr int JUMP_POWER       = 15;

constexpr int MAP_CHIPSIZE = 60;

/////////////////////////
//struct
typedef struct
{
	bool top    = false;
	bool bottom = false;
	bool left   = false;
	bool right  = false;
	Vec2 pos;
}_mapHitState;

////////////////////
//include
#include "class.h"