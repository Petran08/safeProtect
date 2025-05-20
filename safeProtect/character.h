#pragma once
#include <vector>
#include <string>
#include "raylib.h"
#include "projectile.h"
#include "player.h"

void initChar();
void spawnProjectiles(int myId, float playerx, float playery, float mousex, float mousey, player p);

class character
{
public:
	std::string name="name";
	int nrProjectiles=1;
	float hitProjectiles=50;
	float projDist;
	float projSpeed;
	float hitbox;
	std::vector <float> projAngles;
};

extern std::vector <character> chars;
extern std::vector <projectile> proj;