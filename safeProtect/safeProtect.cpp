//my biggest project yet
//that s also the reason for this spagetti code
#include <iostream>
#include <sstream>
#include <cmath>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include "raylib.h"
#include "player.h"
#include "character.h"
#include "projectile.h"
#include "camera.h"
#include "enemy.h"
const int screenWidth = 1000;
const int screenHeight = 600;
const int mapSize = 10000;
short int mapWidth;
short int mapHeight;
const int cellSize = 80;
short int map[135][135];
int myCharId = 0, maxId=4;
double time_elapsed;
int mapId = 1, maxMap=10;
bool editMode = false;
player myPlayer;
character myChar;
//camera myCamera;
Camera2D myNewCamera;
std::vector <enemy> enemies;
Vector2 collPoints[8];
short int collActive[8];

template <typename T>

T clamp(T val, T minVal, T maxVal) {
    if (val < minVal) return minVal;
    if (val > maxVal) return maxVal;
    return val;
}

void intitPlayerCollPoints()
{
    collPoints[0] = { -myPlayer.width / 2, -myPlayer.height / 2 };
    collPoints[1] = { 0, -myPlayer.height / 2 };
    collPoints[2] = { myPlayer.width / 2, -myPlayer.height / 2 };
    collPoints[3] = { myPlayer.width / 2, 0 };
    collPoints[4] = { myPlayer.width / 2, myPlayer.height / 2 };
    collPoints[5] = { 0, myPlayer.height / 2 };
    collPoints[6] = { -myPlayer.width / 2, myPlayer.height / 2 };
    collPoints[7] = { -myPlayer.width / 2, 0 };
}

short int returnTileId(float x, float y)
{
    return map[int(x / cellSize)][int(y / cellSize)];
}

void write_map(int mapId, short int width, short int height)
{
    std::string mapName = "map_" + std::to_string(mapId) + ".txt";
    std::ofstream write(mapName);
    write << '0' << ' ' << width << ' ' << height << '\n'; // write the map size
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            write << map[i][j] << ' ';
        }
        write << '\n';
    }
}

void read_map(int mapId, short int& width, short int& height)
{
    std::string mapName = "map_" + std::to_string(mapId) + ".txt";
    std::ifstream read(mapName);
    //std::ofstream write(mapName);
    int isEmpty;
    read >> isEmpty >> width >> height;
    if (isEmpty == -1)
    {
		//write << '0 ' << width << ' ' << height; // write the map size
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                //map[i][j] = 8;
                map[i][j] = 0;
                //write << '0';
            }
            //write << '\n';
        }
		write_map(mapId, width, height); // write the empty map
    }
    else
    {
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                read >> map[i][j];
            }
        }
    }
}

void empty_map(int mapid, short int& width, short int& height)
{
    std::string mapName = "map_" + std::to_string(mapId) + ".txt";
    std::ofstream write(mapName);
    write << -1 << ' ' << width << ' ' << height << '\n'; // write the map size
    write.close();
    read_map(mapid, width, height);
}

void getKeyboardInput()
{
    int axax = int(IsKeyDown(KEY_D)) - int(IsKeyDown(KEY_A));
    int axay = int(IsKeyDown(KEY_W)) - int(IsKeyDown(KEY_S));
    if (abs(axax) > 0 && abs(axay) > 0)
    {
        if (axax==1 && axay==1)
        {
            myPlayer.angle = PI/4;
        }
        else if (axax == -1 && axay == 1)
        {
            myPlayer.angle = PI/4 + PI/2;
        }
        else if (axax == -1 && axay == -1)
        {
            myPlayer.angle = PI/4 + PI;
        }
        else
        {
            myPlayer.angle = PI/4 + PI/2 + PI;
        }
        myPlayer.isMoving = true;
    }
    else if (abs(axax) > 0 || abs(axay) > 0)
    {
        if (axax == 1)
        {
            myPlayer.angle = 0;
        }
        else if (axax == -1)
        {
            myPlayer.angle = PI;
        }
        else if (axay == 1)
        {
            myPlayer.angle = PI/2;
        }
        else if (axay == -1)
        {
            myPlayer.angle = 3*PI/2;
        }
        myPlayer.isMoving = true;
    }
    else
    {
        myPlayer.isMoving = false;
    }
    if (IsKeyPressed(KEY_SPACE))
    {
        myCharId++;
        if (myCharId > maxId)
            myCharId = 0;
        myChar = chars[myCharId];
        myPlayer.width = myChar.hitbox * 2;
        myPlayer.height = myChar.hitbox * 2;
        intitPlayerCollPoints();
    }
    if (IsKeyPressed(KEY_Z))
    {
        if (myNewCamera.zoom >= 0.4f)
            myNewCamera.zoom -= 0.1f;
        else
            myNewCamera.zoom = 1.5f;
    }
    if (IsKeyPressed(KEY_E))
    {
        if (editMode)
            write_map(mapId, mapWidth, mapHeight);
        editMode = !editMode;
    }
    Vector2 mousePos = GetMousePosition();
    if (IsKeyDown(KEY_ONE) && editMode)
    {
        map[int((myNewCamera.target.x + mousePos.x - screenWidth / 2) / cellSize)][int((myNewCamera.target.y + mousePos.y - screenHeight / 2) / cellSize)] = 0;
    }
    else if (IsKeyDown(KEY_TWO) && editMode)
    {
        map[int((myNewCamera.target.x + mousePos.x - screenWidth / 2) / cellSize)][int((myNewCamera.target.y + mousePos.y - screenHeight / 2) / cellSize)] = 8;
    }
    if (IsKeyPressed(KEY_R) && editMode)
    {
        empty_map(mapId, mapWidth, mapHeight);
    }
    if (IsKeyPressed(KEY_X))
    {
        write_map(mapId, mapWidth, mapHeight);
        mapId++;
        if (mapId > maxMap)
            mapId = 1;
        read_map(mapId, mapWidth, mapHeight);
    }
}

void spawnEnemy(float x, float y, float hit, float size)//not used rn
{
    enemy init;
    init.posx = x;
    init.posy = y;
    init.hitbox = hit;
    init.size = size;
    enemies.push_back(init);
}

void drawBackground()
{
    for (int i = 0; i < mapWidth; i++)
        for (int j = 0; j < mapHeight; j++)
            if (map[i][j] == 8)
                DrawRectangle(i * cellSize, j * cellSize, cellSize, cellSize, BROWN);
            else if ((i + j) % 2 == 0)
                DrawRectangle(i * cellSize, j * cellSize, cellSize, cellSize, GREEN);
            else
                DrawRectangle(i * cellSize, j * cellSize, cellSize, cellSize, DARKGREEN);
}

void drawScreen()
{
    //start rendering

        myNewCamera.target = Vector2{ (float)myPlayer.posx_abs, (float)myPlayer.posy_abs };
        
        // Calculate half of the visible area in world coordinates
        float halfScreenWidth = screenWidth / 2.0f / myNewCamera.zoom;
        float halfScreenHeight = screenHeight / 2.0f / myNewCamera.zoom;

        // Clamp the camera target to map boundaries
        const float cameraMargin = 24.0f / myNewCamera.zoom;

        myNewCamera.target.x = clamp(myNewCamera.target.x, halfScreenWidth, cellSize * mapWidth - halfScreenWidth - cameraMargin);
        myNewCamera.target.y = clamp(myNewCamera.target.y, halfScreenHeight, cellSize * mapHeight - halfScreenHeight - cameraMargin);

        myPlayer.posx_abs = clamp(myPlayer.posx_abs, 0.0f + myPlayer.width / 2, float(cellSize * mapWidth - cameraMargin - myPlayer.width / 2));
        myPlayer.posy_abs = clamp(myPlayer.posy_abs, 0.0f + myPlayer.height / 2, float(cellSize * mapHeight - cameraMargin - myPlayer.height / 2));

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(myNewCamera);

        drawBackground();
        DrawRectangle(myPlayer.posx_abs - myPlayer.width / 2, myPlayer.posy_abs - myPlayer.height / 2, myPlayer.width, myPlayer.height, WHITE);

        // projectiles
        for (int i = 0; i < proj.size(); i++)
        {
            if (proj[i].active)
                DrawCircle(proj[i].x, proj[i].y, proj[i].hitbox, BLUE);
        }

        //collison points
        for (int i = 0; i < 8; i++)
        {
            DrawCircle(myPlayer.posx_abs + collPoints[i].x, myPlayer.posy_abs + collPoints[i].y, 5, RED);
        }

        EndMode2D();
        Vector2 mousePos = GetMousePosition();
        Vector2 worldPos = { myPlayer.posx_coll, myPlayer.posy_coll };
        //Vector2 screenPos = GetWorldToScreen2D(worldPos, myNewCamera); USELESS

        // UI overlays (not affected by camera)
        std::stringstream buffer;
        buffer << "myCharId: " << myCharId;
        buffer << '\n';
        buffer << "Tile Id: " << returnTileId(myPlayer.posx_abs, myPlayer.posy_abs);
        buffer << "\n pos_abs: " << myPlayer.posx_abs << " " << myPlayer.posy_abs;
        buffer << "\n pos_coll: " << myPlayer.posx_coll << " " << myPlayer.posy_coll;
        buffer << "\n camera target: " << myNewCamera.target.x << " " << myNewCamera.target.y;
        buffer << "\n time elapsed: " << time_elapsed;
        buffer << "\n mouse position: " << mousePos.x << " " << mousePos.y;
        buffer << "\n zoom: " << myNewCamera.zoom;
        buffer << "\n edit: " << bool(editMode);
        //buffer << "\n on screen pos: " << screenPos.x << " " << screenPos.y; USELESS
		buffer << "\n mapId: " << mapId;
        DrawText(buffer.str().c_str(), 10, 10, 30, BLACK);

        EndDrawing();

		//end rendering
}


void checkCollision()
{
    int a = 0;
    for (int i = 0; i < 8; i++)
    {
        if (returnTileId(myPlayer.posx_abs + collPoints[i].x, myPlayer.posy_abs + collPoints[i].y) == 8)
        {
            collActive[i] = 1;
        }
        else
        {
            collActive[i] = 0;
        }
    }
    
}

void movePlayer()
{
    if (myPlayer.isMoving)
    {
        myPlayer.posx_abs += (myPlayer.speed) * cos(myPlayer.angle);
        //myPlayer.posx_abs = int(myPlayer.posx_abs);
        //if(myPlayer.posx_abs>=screenWidth/2)
           //myCamera.posx = myPlayer.posx_abs;
        
        myPlayer.posy_abs -= (myPlayer.speed) * sin(myPlayer.angle);
        //myPlayer.posy_abs = int(myPlayer.posy_abs);

        const float cameraMargin = 24.0f / myNewCamera.zoom;

        //if(myPlayer.posy_abs>=screenHeight/2)
            //myCamera.posy = myPlayer.posy_abs;
    }
    if(!editMode)
    {
        checkCollision();
        if (collActive[1] == 1 || collActive[3] == 1 || collActive[5] == 1 || collActive[7] == 1)
        {
            if (collActive[1] == 1)
            {
                while (returnTileId(myPlayer.posx_abs + collPoints[1].x, myPlayer.posy_abs + collPoints[1].y) == 8)
                {
                    myPlayer.posy_abs += 1;
                }
            }
            if (collActive[3] == 1)
            {
                while (returnTileId(myPlayer.posx_abs + collPoints[3].x, myPlayer.posy_abs + collPoints[3].y) == 8)
                {
                    myPlayer.posx_abs -= 1;
                }
            }
            if (collActive[5] == 1)
            {
                while (returnTileId(myPlayer.posx_abs + collPoints[5].x, myPlayer.posy_abs + collPoints[5].y) == 8)
                {
                    myPlayer.posy_abs -= 1;
                }
            }
            if (collActive[7] == 1)
            {
                while (returnTileId(myPlayer.posx_abs + collPoints[7].x, myPlayer.posy_abs + collPoints[7].y) == 8)
                {
                    myPlayer.posx_abs += 1;
                }
            }
        }
        else
        {
            srand(time(0));
            int rndnr = rand(); // worst way to implement collision, but it will do for now
            //I WILL change it later
            if (myPlayer.angle == 0 || myPlayer.angle == PI || myPlayer.angle == PI / 2 || myPlayer.angle == 3 * PI / 2)
            {
                while (returnTileId(myPlayer.posx_abs + collPoints[0].x, myPlayer.posy_abs + collPoints[0].y) == 8 || returnTileId(myPlayer.posx_abs + collPoints[2].x, myPlayer.posy_abs + collPoints[2].y) == 8 || returnTileId(myPlayer.posx_abs + collPoints[4].x, myPlayer.posy_abs + collPoints[4].y) == 8 || returnTileId(myPlayer.posx_abs + collPoints[6].x, myPlayer.posy_abs + collPoints[6].y) == 8)
                {
                    myPlayer.posx_abs += (2) * cos(myPlayer.angle + PI);
                    myPlayer.posx_abs = int(myPlayer.posx_abs);
                    myPlayer.posy_abs -= (2) * sin(myPlayer.angle + PI);
                    myPlayer.posy_abs = int(myPlayer.posy_abs);
                }
            }
            else if (myPlayer.angle = PI / 4)
            {
                if (returnTileId(myPlayer.posx_abs + collPoints[0].x, myPlayer.posy_abs + collPoints[0].y) == 8)
                {
                    while (returnTileId(myPlayer.posx_abs + collPoints[0].x, myPlayer.posy_abs + collPoints[0].y) == 8)
                    {
                        myPlayer.posy_abs += (1);
                    }
                }
                else if (returnTileId(myPlayer.posx_abs + collPoints[4].x, myPlayer.posy_abs + collPoints[4].y) == 8)
                {
                    while (returnTileId(myPlayer.posx_abs + collPoints[4].x, myPlayer.posy_abs + collPoints[4].y) == 8)
                    {
                        myPlayer.posx_abs -= (1);
                    }
                }
                else if (returnTileId(myPlayer.posx_abs + collPoints[2].x, myPlayer.posy_abs + collPoints[2].y) == 8)
                {
                    Vector2 point1 = { myPlayer.posx_abs + collPoints[2].x,   myPlayer.posy_abs + collPoints[2].y }; //variable x
                    Vector2 point2 = point1; // variable y
                    int onx = 0, ony = 0;
                    while (returnTileId(myPlayer.posx_abs + point1.x, myPlayer.posy_abs + point1.y) == 8 && returnTileId(myPlayer.posx_abs + point2.x, myPlayer.posy_abs + point2.y) == 8)
                    {
                        point1.x -= 0.09f;
                        point2.y += 0.09f;
                    }

                    if (returnTileId(myPlayer.posx_abs + point1.x, myPlayer.posy_abs + point1.y) != 8 && returnTileId(myPlayer.posx_abs + point2.x, myPlayer.posy_abs + point2.y) != 8)
                    {
                        onx = 1;
                        ony = 0;
                    }
                    else if (returnTileId(myPlayer.posx_abs + point1.x, myPlayer.posy_abs + point1.y) != 8)
                    {
                        onx = 1;
                    }
                    else if (returnTileId(myPlayer.posx_abs + point2.x, myPlayer.posy_abs + point2.y) != 8)
                    {
                        ony = 1;
                    }

                    while (returnTileId(myPlayer.posx_abs + collPoints[2].x, myPlayer.posy_abs + collPoints[2].y) == 8)
                    {
                        myPlayer.posx_abs -= (1) * onx;
                        myPlayer.posy_abs += (1) * ony;
                    }
                }
            }
            else if (myPlayer.angle = 3 * PI / 4)
            {
                if (returnTileId(myPlayer.posx_abs + collPoints[2].x, myPlayer.posy_abs + collPoints[2].y) == 8)
                {
                    while (returnTileId(myPlayer.posx_abs + collPoints[2].x, myPlayer.posy_abs + collPoints[2].y) == 8)
                    {
                        myPlayer.posy_abs += (1);
                    }
                }
                else if (returnTileId(myPlayer.posx_abs + collPoints[6].x, myPlayer.posy_abs + collPoints[6].y) == 8)
                {
                    while (returnTileId(myPlayer.posx_abs + collPoints[6].x, myPlayer.posy_abs + collPoints[6].y) == 8)
                    {
                        myPlayer.posx_abs += (1);
                    }
                }
                else if (returnTileId(myPlayer.posx_abs + collPoints[0].x, myPlayer.posy_abs + collPoints[0].y) == 8)
                {
                    Vector2 point1 = { myPlayer.posx_abs + collPoints[0].x,   myPlayer.posy_abs + collPoints[0].y }; //variable x
                    Vector2 point2 = point1; // variable y
                    int onx = 0, ony = 0;
                    while (returnTileId(myPlayer.posx_abs + point1.x, myPlayer.posy_abs + point1.y) == 8 && returnTileId(myPlayer.posx_abs + point2.x, myPlayer.posy_abs + point2.y) == 8)
                    {
                        point1.x += 0.09f;
                        point2.y += 0.09f;
                    }

                    if (returnTileId(myPlayer.posx_abs + point1.x, myPlayer.posy_abs + point1.y) != 8 && returnTileId(myPlayer.posx_abs + point2.x, myPlayer.posy_abs + point2.y) != 8)
                    {
                        onx = 1;
                        ony = 0;
                    }
                    else if (returnTileId(myPlayer.posx_abs + point1.x, myPlayer.posy_abs + point1.y) != 8)
                    {
                        onx = 1;
                    }
                    else if (returnTileId(myPlayer.posx_abs + point2.x, myPlayer.posy_abs + point2.y) != 8)
                    {
                        ony = 1;
                    }

                    while (returnTileId(myPlayer.posx_abs + collPoints[0].x, myPlayer.posy_abs + collPoints[0].y) == 8)
                    {
                        myPlayer.posx_abs += (1) * onx;
                        myPlayer.posy_abs += (1) * ony;
                    }
                }
            }
            else if (myPlayer.angle = 5 * PI / 4)
            {
                if (returnTileId(myPlayer.posx_abs + collPoints[4].x, myPlayer.posy_abs + collPoints[4].y) == 8)
                {
                    while (returnTileId(myPlayer.posx_abs + collPoints[4].x, myPlayer.posy_abs + collPoints[4].y) == 8)
                    {
                        myPlayer.posy_abs -= (1);
                    }
                }
                else if (returnTileId(myPlayer.posx_abs + collPoints[0].x, myPlayer.posy_abs + collPoints[0].y) == 8)
                {
                    while (returnTileId(myPlayer.posx_abs + collPoints[0].x, myPlayer.posy_abs + collPoints[0].y) == 8)
                    {
                        myPlayer.posx_abs += (1);
                    }
                }
                else if (returnTileId(myPlayer.posx_abs + collPoints[6].x, myPlayer.posy_abs + collPoints[6].y) == 8)
                {
                    Vector2 point1 = { myPlayer.posx_abs + collPoints[6].x,   myPlayer.posy_abs + collPoints[6].y }; //variable x
                    Vector2 point2 = point1; // variable y
                    int onx = 0, ony = 0;
                    while (returnTileId(myPlayer.posx_abs + point1.x, myPlayer.posy_abs + point1.y) == 8 && returnTileId(myPlayer.posx_abs + point2.x, myPlayer.posy_abs + point2.y) == 8)
                    {
                        point1.x += 0.09f;
                        point2.y -= 0.09f;
                    }

                    if (returnTileId(myPlayer.posx_abs + point1.x, myPlayer.posy_abs + point1.y) != 8 && returnTileId(myPlayer.posx_abs + point2.x, myPlayer.posy_abs + point2.y) != 8)
                    {
                        onx = 1;
                        ony = 0;
                    }
                    else if (returnTileId(myPlayer.posx_abs + point1.x, myPlayer.posy_abs + point1.y) != 8)
                    {
                        onx = 1;
                    }
                    else if (returnTileId(myPlayer.posx_abs + point2.x, myPlayer.posy_abs + point2.y) != 8)
                    {
                        ony = 1;
                    }

                    while (returnTileId(myPlayer.posx_abs + collPoints[6].x, myPlayer.posy_abs + collPoints[6].y) == 8)
                    {
                        myPlayer.posx_abs += (1) * onx;
                        myPlayer.posy_abs -= (1) * ony;
                    }
                }
            }
            else if (myPlayer.angle = 7 * PI / 4)
            {
                if (returnTileId(myPlayer.posx_abs + collPoints[6].x, myPlayer.posy_abs + collPoints[6].y) == 8)
                {
                    while (returnTileId(myPlayer.posx_abs + collPoints[6].x, myPlayer.posy_abs + collPoints[6].y) == 8)
                    {
                        myPlayer.posy_abs -= (1);
                    }
                }
                else if (returnTileId(myPlayer.posx_abs + collPoints[2].x, myPlayer.posy_abs + collPoints[2].y) == 8)
                {
                    while (returnTileId(myPlayer.posx_abs + collPoints[2].x, myPlayer.posy_abs + collPoints[2].y) == 8)
                    {
                        myPlayer.posx_abs -= (1);
                    }
                }
                else if (returnTileId(myPlayer.posx_abs + collPoints[4].x, myPlayer.posy_abs + collPoints[4].y) == 8)
                {
                    Vector2 point1 = { myPlayer.posx_abs + collPoints[4].x,   myPlayer.posy_abs + collPoints[4].y }; //variable x
                    Vector2 point2 = point1; // variable y
                    int onx = 0, ony = 0;
                    while (returnTileId(myPlayer.posx_abs + point1.x, myPlayer.posy_abs + point1.y) == 8 && returnTileId(myPlayer.posx_abs + point2.x, myPlayer.posy_abs + point2.y) == 8)
                    {
                        point1.x -= 0.09f;
                        point2.y -= 0.09f;
                    }

                    if (returnTileId(myPlayer.posx_abs + point1.x, myPlayer.posy_abs + point1.y) != 8 && returnTileId(myPlayer.posx_abs + point2.x, myPlayer.posy_abs + point2.y) != 8)
                    {
                        onx = 1;
                        ony = 0;
                    }
                    else if (returnTileId(myPlayer.posx_abs + point1.x, myPlayer.posy_abs + point1.y) != 8)
                    {
                        onx = 1;
                    }
                    else if (returnTileId(myPlayer.posx_abs + point2.x, myPlayer.posy_abs + point2.y) != 8)
                    {
                        ony = 1;
                    }

                    while (returnTileId(myPlayer.posx_abs + collPoints[4].x, myPlayer.posy_abs + collPoints[4].y) == 8)
                    {
                        myPlayer.posx_abs -= (1) * onx;
                        myPlayer.posy_abs -= (1) * ony;
                    }
                }
            }
        }
    }

    myPlayer.posx_rel = (myPlayer.posx_abs - myNewCamera.target.x) * myNewCamera.zoom + screenWidth / 2;
    myPlayer.posy_rel = (myPlayer.posy_abs - myNewCamera.target.y) * myNewCamera.zoom + screenHeight / 2;
        
    myPlayer.posx_coll = myPlayer.posx_rel;
    myPlayer.posy_coll = myPlayer.posy_rel;
}

//TODO fix the mouse position shits when zoomed out(i hope this is the only problem)(its not the only problem, IM DONE FOR TODAY (no proj rendering, division by 0(fix this)))
//i fixed everything wrong(i hope)

void getMouseInput()
{
    bool truem = IsMouseButtonPressed(MOUSE_BUTTON_LEFT), truen = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
    if (truem || IsKeyPressed(KEY_C))// for debugging reasons
    {
        //Vector2 mousePos = GetMousePosition();
        Vector2 mousePos = GetMousePosition();
        Vector2 worldPos = { myPlayer.posx_coll, myPlayer.posy_coll };
        //Vector2 screenPos = GetWorldToScreen2D(worldPos, myNewCamera);//useless
        //std::cout << screenPos.x << " " << screenPos.y << '\n';
        spawnProjectiles(myCharId, worldPos.x, worldPos.y, float(mousePos.x), float(mousePos.y), myPlayer, "attack");
    }
    else if (truen || IsKeyPressed(KEY_V))
    {
        //Vector2 mousePos = GetMousePosition();
        Vector2 mousePos = GetMousePosition();
        Vector2 worldPos = { myPlayer.posx_coll, myPlayer.posy_coll };
        //Vector2 screenPos = GetWorldToScreen2D(worldPos, myNewCamera);//useless
        superAttack(myCharId, worldPos.x, worldPos.y, float(mousePos.x), float(mousePos.y), myPlayer, time_elapsed);
    }
}

void moveProjectiles()
{
    for (int i = 0; i < proj.size(); i++)
    {
        if (proj[i].active)
        {
            if (proj[i].distance <= 0)
                proj[i].active = false;
            proj[i].x += proj[i].speed * cos(proj[i].angle);
            proj[i].y -= proj[i].speed * sin(proj[i].angle);
            proj[i].distance -= proj[i].speed;
        }
    }
}

/*void mapinit()
{
    map[0][0] = 8;
    map[4][7] = 8;
    map[10][14] = 8;
}*/

int main()
{
    InitWindow(screenWidth, screenHeight, "Safe Protector");
    SetTargetFPS(60);
    initChar();
    read_map(mapId, mapWidth, mapHeight);
    myChar = chars[myCharId];
    myPlayer.width = myChar.hitbox * 2;
    myPlayer.height = myChar.hitbox * 2;
    //myCamera.posx = myPlayer.posx_abs;
    //myCamera.posy = myPlayer.posy_abs;
    
    intitPlayerCollPoints();

    myNewCamera.target = Vector2{ (float)myPlayer.posx_abs, (float)myPlayer.posy_abs };
    myNewCamera.offset = Vector2{ screenWidth / 2.0f,screenHeight / 2.0f };
    myNewCamera.rotation = 0.0f;
    myNewCamera.zoom = 1.0f;
   
    myPlayer.posx_rel = myPlayer.posx_abs - myNewCamera.target.x + screenWidth / 2;
    myPlayer.posy_rel = myPlayer.posy_abs - myNewCamera.target.y + screenHeight / 2;

    myPlayer.posx_coll = myPlayer.posx_rel;
    myPlayer.posy_coll = myPlayer.posy_rel;
    //begin time here
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start;
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    time_elapsed += duration.count() / 1000.0;
    while (!WindowShouldClose())
    {
        getKeyboardInput();
        getMouseInput();
        movePlayer();
        moveProjectiles();
        drawScreen();

        //end it here
        end = std::chrono::high_resolution_clock::now();
        //calculate time passed and use it for effects and spawnables attacks
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        time_elapsed += duration.count() / 1000.0;
        //start time here
        start = std::chrono::high_resolution_clock::now();
    }
    write_map(mapId, mapWidth, mapHeight);
    CloseWindow();
    return 0;
}