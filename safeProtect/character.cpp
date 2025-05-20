#include <cmath>
#include <iostream>
#include "character.h"
#include "projectile.h"
#include "player.h"

std::vector <character> chars;
std::vector <projectile> proj;

character init;
projectile initProj;

 void initChar()
{
	 init.name = "Starter";
	 init.nrProjectiles = 1;
	 init.hitProjectiles = 50;
	 init.hitbox = 50;
	 init.projAngles.push_back(0);
	 init.projDist = 750;
	 init.projSpeed = 20;
	 chars.push_back(init);
}

 void spawnProjectiles(int myId, float playerx, float playery, float mousex, float mousey, player p)
 {
	 for (int i = 0; i < chars[myId].nrProjectiles; i++)
	 {
		 float x = mousex - playerx, y = mousey - playery;
		 float calcAngle, tn=0;
		 if (x == 0)
			 if (y > 0)
				 calcAngle = 3 * PI / 2;
			 else
				 calcAngle = PI / 2;
		 else
		 {
			 tn = abs(y) / abs(x);
			 calcAngle = atan(tn);
			 if (x < 0 && y < 0)
				 calcAngle = PI - calcAngle;
			 else if (x < 0 && y>0)
				 calcAngle += PI;
			 else if (x > 0 && y > 0)
				 calcAngle = 2 * PI - calcAngle;
		 }
		 
		 initProj.angle = chars[myId].projAngles[i] + calcAngle;
		 //std::cout << calcAngle << " " << tn << '\n';
		 initProj.speed = chars[myId].projSpeed;
		 initProj.hitbox = 20;
		 initProj.x = p.posx;
		 initProj.y = p.posy;
		 initProj.distance = chars[myId].projDist;
		 bool empty_spot = false;
		 for (long long i = 0; i < proj.size() && !empty_spot; i++)
		 {
			 if (!proj[i].active)
			 {
				 proj[i] = initProj;
				 empty_spot = true;
			 }
		 }
		 if(!empty_spot)
			proj.push_back(initProj);
	 }
	 
 }
