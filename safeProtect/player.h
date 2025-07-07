#pragma once
#include "raylib.h"
class player
{
public:
	float posx_coll;//colision UNUSED
	float posy_coll;//colision UNUSED
	float posx_rel = 500;//position on screen(whare it renderes(from te up-left corner)) UNUSED
	float posy_rel = 300;//position on screen(whare it renderes(from te up-left corner)) UNUSED
	float posx_abs = 500;//general position(in the middle of the character)
	float posy_abs = 300;//general position(in the middle of the character)
	float width;
	float height;
	float angle = 0;
	float oSpeed = 10;
	float speed = 10;
	bool isMoving = false;
};

