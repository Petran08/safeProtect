#include <cmath>
#include <iostream>
#include "character.h"
#include "projectile.h"
#include "player.h"
#include "spawnable.h"

std::vector <character> chars;
std::vector <projectile> proj;
std::vector <spawnable> spwn_info;
std::vector <spawnable> spwn_render;

character init;
projectile initProj;
csv_file char_data;
csv_file spwn_data;

float degToRad(float angle)
{

	return angle / 180 * PI;
}

int stringToNr(std::string str)
{
	int nr = 0, i=0, m=1;
	if (str[0] == '0' && str[1] == '0')
	{
		i = 2;
		m = -1;
	}
	for (i = i; i < str.length(); i++)
	{
		nr = nr * 10 + (str[i] - '0');
	}
	return nr*m;
}

 void initChar()
{
	 char_data.file_name = "data_char.csv";
	 char_data.read_file();
	 for (int i = 0; i < char_data.rows_used; i++)
	 {
		 std::cout << i << '\n';
		 init.name = char_data.contents[i][0];
		 init.nrProjectiles = stringToNr(char_data.contents[i][1]);
		 std::string val = char_data.contents[i][2];
		 for (int j = 0; j < init.nrProjectiles; j+=0)
		 {
			 std::string v;
			 for (int a = 0; a < val.length(); a++)
			 {
				 if (val[a] == '|')
					 j++, init.hitProjectiles.push_back(stringToNr(v)), v = "";
				 else
					 v.push_back(val[a]);
			 }
		 }
		 init.hitbox = stringToNr(char_data.contents[i][3]);
		 val = char_data.contents[i][4];
		 for (int j = 0; j < init.nrProjectiles; j += 0)
		 {
			 std::string v;
			 for (int a = 0; a < val.length(); a++)
			 {
				 if (val[a] == '|')
					 j++, init.projAngles.push_back(degToRad(stringToNr(v))), v = "";
				 else
					 v.push_back(val[a]);
			 }
		 }
		 val = char_data.contents[i][5];
		 for (int j = 0; j < init.nrProjectiles; j += 0)
		 {
			 std::string v;
			 for (int a = 0; a < val.length(); a++)
			 {
				 if (val[a] == '|')
					 j++, init.projSpeed.push_back(stringToNr(v)), v = "";
				 else
					 v.push_back(val[a]);
			 }
		 }
		 val = char_data.contents[i][6];
		 for (int j = 0; j < init.nrProjectiles; j += 0)
		 {
			 std::string v;
			 for (int a = 0; a < val.length(); a++)
			 {
				 if (val[a] == '|')
					 j++, init.projDist.push_back(stringToNr(v)), v = "";
				 else
					 v.push_back(val[a]);
			 }
		 }
		 val = char_data.contents[i][7];
		 for (int j = 0; j < init.nrProjectiles; j += 0)
		 {
			 std::string v;
			 for (int a = 0; a < val.length(); a++)
			 {
				 if (val[a] == '|')
					 j++, init.damage.push_back(stringToNr(v)), v = "";
				 else
					 v.push_back(val[a]);
			 }
		 }
		 init.hp = stringToNr(char_data.contents[i][8]);
		 init.newId = stringToNr(char_data.contents[i][9]);
		 init.sAttType = char_data.contents[i][10];
		 init.sNrProj = stringToNr(char_data.contents[i][11]);
		 val = char_data.contents[i][12];
		 for (int j = 0; j < init.nrProjectiles; j += 0)
		 {
			 std::string v;
			 for (int a = 0; a < val.length(); a++)
			 {
				 if (val[a] == '|')
					 j++, init.sHitProj.push_back(stringToNr(v)), v = "";
				 else
					 v.push_back(val[a]);
			 }
		 }
		 val = char_data.contents[i][13];
		 for (int j = 0; j < init.nrProjectiles; j += 0)
		 {
			 std::string v;
			 for (int a = 0; a < val.length(); a++)
			 {
				 if (val[a] == '|')
					 j++, init.sProjAngles.push_back(degToRad(stringToNr(v))), v = "";
				 else
					 v.push_back(val[a]);
			 }
		 }
		 val = char_data.contents[i][14];
		 for (int j = 0; j < init.nrProjectiles; j += 0)
		 {
			 std::string v;
			 for (int a = 0; a < val.length(); a++)
			 {
				 if (val[a] == '|')
					 j++, init.sProjDist.push_back(stringToNr(v)), v = "";
				 else
					 v.push_back(val[a]);
			 }
		 }
		 val = char_data.contents[i][15];
		 for (int j = 0; j < init.nrProjectiles; j += 0)
		 {
			 std::string v;
			 for (int a = 0; a < val.length(); a++)
			 {
				 if (val[a] == '|')
					 j++, init.sProjDmg.push_back(stringToNr(v)), v = "";
				 else
					 v.push_back(val[a]);
			 }
		 }
		 val = char_data.contents[i][16];
		 for (int j = 0; j < init.nrProjectiles; j += 0)
		 {
			 std::string v;
			 for (int a = 0; a < val.length(); a++)
			 {
				 if (val[a] == '|')
					 j++, init.sProjSpeed.push_back(stringToNr(v)), v = "";
				 else
					 v.push_back(val[a]);
			 }
		 }
		 init.efDur = stringToNr(char_data.contents[i][17]);
		 init.ef = char_data.contents[i][18];
		 init.efVal = stringToNr(char_data.contents[i][19]);
		 init.spwnId = stringToNr(char_data.contents[i][20]);
		 init.sNewId = stringToNr(char_data.contents[i][21]);
		 chars.push_back(init);
		 init.hitProjectiles.clear();
		 init.projSpeed.clear();
		 init.projDist.clear();
		 init.projAngles.clear();
		 init.damage.clear();
		 init.sHitProj.clear();
		 init.sProjSpeed.clear();
		 init.sProjDist.clear();
		 init.sProjAngles.clear();
		 init.sProjDmg.clear();
	 }
}

 void spawnProjectiles(int& myId, float playerx, float playery, float mousex, float mousey, player p, std::string type)
 {
	 if (type == "attack")
	 {
		 for (int i = 0; i < chars[myId].nrProjectiles; i++)
		 {
			 float x = mousex - playerx, y = mousey - playery;
			 float calcAngle, tn = 0;
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
			 initProj.speed = chars[myId].projSpeed[i];
			 initProj.hitbox = chars[myId].hitProjectiles[i];
			 initProj.x = p.posx_abs;
			 initProj.y = p.posy_abs;
			 initProj.distance = chars[myId].projDist[i];
			 bool empty_spot = false;
			 for (long long i = 0; i < proj.size() && !empty_spot; i++)
			 {
				 if (!proj[i].active)
				 {
					 proj[i] = initProj;
					 empty_spot = true;
				 }
			 }
			 if (!empty_spot && proj.size() <= 4000)
				 proj.push_back(initProj);
			 std::cout << proj.size() << '\n';
		 }
		 myId = chars[myId].newId;
	 }
	 else if (type == "super")
	 {
		 for (int i = 0; i < chars[myId].sNrProj; i++)
		 {
			 float x = mousex - playerx, y = mousey - playery;
			 float calcAngle, tn = 0;
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
			 initProj.angle = chars[myId].sProjAngles[i] + calcAngle;
			 //std::cout << calcAngle << " " << tn << '\n';
			 initProj.speed = chars[myId].sProjSpeed[i];
			 initProj.hitbox = chars[myId].sHitProj[i];
			 initProj.x = p.posx_abs;
			 initProj.y = p.posy_abs;
			 initProj.distance = chars[myId].sProjDist[i];
			 bool empty_spot = false;
			 for (long long i = 0; i < proj.size() && !empty_spot; i++)
			 {
				 if (!proj[i].active)
				 {
					 proj[i] = initProj;
					 empty_spot = true;
				 }
			 }
			 if (!empty_spot && proj.size() <= 4000)
				 proj.push_back(initProj);
			 std::cout << proj.size() << '\n';
		 }
		 myId = chars[myId].sNewId;
	 }
 }

 void superAttack(int& myId, float playerx, float playery, float mousex, float mousey, player p)
 {
	 if (chars[myId].sAttType == "proj")
	 {
		 spawnProjectiles(myId, playerx, playery, mousex, mousey, p, "super");
	 }
 }
