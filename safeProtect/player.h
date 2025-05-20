#pragma once
#include "raylib.h"
class player
{
public:
	float posx_coll;//colision
	float posy_coll;//colision
	float posx_rel = 500;//position on screen(whare it renderes(from te up-left corner))
	float posy_rel = 300;//position on screen(whare it renderes(from te up-left corner))
	float posx_abs = 500;
	float posy_abs = 300;
	float width;
	float height;
	float angle = 0;
	float speed = 10;
	bool isMoving = false;
};

