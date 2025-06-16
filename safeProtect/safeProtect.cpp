//my biggest project yet
//that s also the reason for this spagetti code
#include <iostream>
#include <sstream>
#include <cmath>
#include <chrono>
#include "raylib.h"
#include "player.h"
#include "character.h"
#include "projectile.h"
#include "camera.h"
#include "enemy.h"
const int screenWidth = 1000;
const int screenHeight = 600;
const int mapSize = 10000;
const int cellSize = 75;
short int map[135][135];
int myCharId = 0, maxId=4;
double time_elapsed;
player myPlayer;
character myChar;
camera myCamera;
std::vector <enemy> enemies;

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
    for (int i = 0; i < mapSize / cellSize; i++)
        for (int j = 0; j < mapSize / cellSize; j++)
            if (map[i][j] == 8)
                DrawRectangle(i * cellSize - myCamera.posx + screenWidth / 2, j * cellSize - myCamera.posy + screenHeight / 2, cellSize, cellSize, BROWN);
            else if ((i + j) % 2 == 0)
                DrawRectangle(i * cellSize - myCamera.posx + screenWidth / 2, j * cellSize - myCamera.posy + screenHeight / 2, cellSize, cellSize, GREEN);
            else
                DrawRectangle(i * cellSize - myCamera.posx + screenWidth / 2, j * cellSize - myCamera.posy + screenHeight / 2, cellSize, cellSize, DARKGREEN);
}

void drawScreen()
{
    ClearBackground(BLACK);
    drawBackground();
    DrawRectangle(myPlayer.posx_rel, myPlayer.posy_rel, myPlayer.width, myPlayer.height, WHITE);
    for (int i = 0; i < proj.size(); i++)
    {
        if(proj[i].active)
            DrawCircle(proj[i].x - myCamera.posx + screenWidth / 2, proj[i].y - myCamera.posy + screenHeight / 2, proj[i].hitbox, BLUE);
    }
    std::stringstream buffer;
    buffer << "myCharId: " << myCharId;
    buffer << '\n';
    buffer << "Tile Id: " << map[int(myPlayer.posx_abs / cellSize)][int(myPlayer.posy_abs / cellSize)];
    buffer << "\n pos_abs: " << myPlayer.posx_abs << " " << myPlayer.posy_abs;
    buffer << "\n pos_rel: " << myPlayer.posx_rel << " " << myPlayer.posy_rel;
    buffer << "\n pos_coll: " << myPlayer.posx_coll << " " << myPlayer.posy_coll;
    buffer << "\n time elapsed: " << time_elapsed;
    DrawText(buffer.str().c_str(), 10, 10, 30, BLACK);
}

void movePlayer()
{
    if (myPlayer.isMoving)
    {
        myPlayer.posx_abs += (myPlayer.speed) * cos(myPlayer.angle);
        myPlayer.posx_abs = int(myPlayer.posx_abs);
        if(myPlayer.posx_abs>=screenWidth/2)
            myCamera.posx = myPlayer.posx_abs;
        
        myPlayer.posy_abs -= (myPlayer.speed) * sin(myPlayer.angle);
        myPlayer.posy_abs = int(myPlayer.posy_abs);
        if(myPlayer.posy_abs>=screenHeight/2)
            myCamera.posy = myPlayer.posy_abs;
        myPlayer.posx_rel = myPlayer.posx_abs - myCamera.posx + screenWidth / 2 - myPlayer.width / 2;
        myPlayer.posy_rel = myPlayer.posy_abs - myCamera.posy + screenHeight / 2 - myPlayer.height / 2;
        
        myPlayer.posx_coll = myPlayer.posx_rel + myPlayer.width / 2;
        myPlayer.posy_coll = myPlayer.posy_rel + myPlayer.height / 2;
    }
    
}

void getMouseInput()
{
    bool truem = IsMouseButtonPressed(MOUSE_BUTTON_LEFT), truen = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
    if (truem || IsKeyPressed(KEY_C))// for debugging reasons
    {
        Vector2 mousePos = GetMousePosition();
        spawnProjectiles(myCharId, myPlayer.posx_coll, myPlayer.posy_coll, float(mousePos.x), float(mousePos.y), myPlayer, "attack");
    }
    else if (truen || IsKeyPressed(KEY_V))
    {
        Vector2 mousePos = GetMousePosition();
        superAttack(myCharId, myPlayer.posx_coll, myPlayer.posy_coll, float(mousePos.x), float(mousePos.y), myPlayer);
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

void mapinit()
{
    map[0][0] = 8;
    map[4][7] = 8;
    map[10][14] = 8;
}

int main()
{
    InitWindow(screenWidth, screenHeight, "Safe Protector");
    SetTargetFPS(60);
    initChar();
    mapinit();
    time_t t = time(NULL);
    std::cout << t << '\n';
    myChar = chars[myCharId];
    myPlayer.width = myChar.hitbox * 2;
    myPlayer.height = myChar.hitbox * 2;
    myCamera.posx = myPlayer.posx_abs;
    myCamera.posy = myPlayer.posy_abs;
    myPlayer.posx_rel = myPlayer.posx_abs - myCamera.posx + screenWidth / 2 - myPlayer.width / 2;
    myPlayer.posy_rel = myPlayer.posy_abs - myCamera.posy + screenHeight / 2 - myPlayer.height / 2;
    myPlayer.posx_coll = myPlayer.posx_rel + myPlayer.width / 2;
    myPlayer.posy_coll = myPlayer.posy_rel + myPlayer.height / 2;
    //begin time here
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start;
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    time_elapsed += duration.count() / 1000.0;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        getKeyboardInput();
        getMouseInput();
        movePlayer();
        moveProjectiles();
        drawScreen();
        EndDrawing();
        //end it here
        end = std::chrono::high_resolution_clock::now();
        //calculate time passed and use it for effects and spawnables attacks
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        time_elapsed += duration.count() / 1000.0;
        //start time here
        start = std::chrono::high_resolution_clock::now();
    }
    CloseWindow();
    return 0;
}