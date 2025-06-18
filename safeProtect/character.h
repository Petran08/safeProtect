#pragma once
#include <vector>
#include <string>
#include "raylib.h"
#include "projectile.h"
#include "player.h"
#include "csv_file.h"

float degToRad(float angle);

void initChar();
void spawnProjectiles(int& myId, float playerx, float playery, float mousex, float mousey, player p, std::string type);
void superAttack(int& myId, float playerx, float playery, float mousex, float mousey, player p, double moment_activated);



class character
{
public:
	std::string name="name";
	int hp;
	int nrProjectiles=1;
	std::vector <float> hitProjectiles;
	std::vector <float> projDist;
	std::vector <float> projSpeed;
	float hitbox;
	std::vector <float> projAngles;
	std::vector <int> damage;
	int newId;//(after attack)
	int sNrProj = 1;
	std::vector <float> sHitProj;
	std::vector <float> sProjDist;
	std::vector <float> sProjSpeed;
	std::vector <float> sProjAngles;
	std::vector <int> sProjDmg;
	int sNewId;//(after super)
	std::string sAttType;
	float efDur;
	std::string ef;
	float efVal;
	int spwnId;
};

extern std::vector <character> chars;
extern std::vector <projectile> proj;
extern csv_file char_data;