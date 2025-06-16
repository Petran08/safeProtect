#pragma once
#include <string>
#include <vector>
#include "csv_file.h"

void spawnSpwn(int posx, int posy, int id);

class spawnable
{
public:
	int hp;
	std::string attack_type;
	int id;
	int hp_decrease;
	int nrProjectiles = 1;
	std::vector <float> hitProjectiles;
	std::vector <float> projDist;
	std::vector <float> projSpeed;
	float hitbox;
	std::vector <float> projAngles;
	std::vector <int> damage;
};

extern csv_file spwn_data;
extern std::vector <spawnable> spwn_info;
extern std::vector <spawnable> spwn_render;

