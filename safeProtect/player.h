#pragma once
#include "raylib.h"
class player
{
public:
	float posx = 500;//useless
	float posy = 300;//useless
	float posx_rel = 500;
	float posy_rel = 300;
	float posx_abs = 500;
	float posy_abs = 300;
	float width;
	float height;
	float angle = 0;
	float speed = 10;
	bool isMoving = false;
};

