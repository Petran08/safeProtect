#include <iostream>
#include <cmath>
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
int myCharId = 0, oldCharId = 0;
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
}

void spawnEnemy(float x, float y, float hit, float size)
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
    DrawRectangle(myPlayer.posx - myCamera.posx + screenWidth/2 - myPlayer.width/2, myPlayer.posy - myCamera.posy + screenHeight/2 - myPlayer.height/2, myPlayer.width, myPlayer.height, WHITE);
    for (int i = 0; i < proj.size(); i++)
    {
        if(proj[i].active)
            DrawCircle(proj[i].x - myCamera.posx + screenWidth / 2, proj[i].y - myCamera.posy + screenHeight / 2, proj[i].hitbox, BLUE);
    }
}

void movePlayer()
{
    if (myPlayer.isMoving)
    {
        myPlayer.posx += myPlayer.speed * cos(myPlayer.angle);
        if(myPlayer.posx>=screenWidth/2)
            myCamera.posx = myPlayer.posx;
        myPlayer.posy -= myPlayer.speed * sin(myPlayer.angle);
        if(myPlayer.posy>=screenHeight/2)
            myCamera.posy = myPlayer.posy;
    }
}

void getMouseInput()
{
    bool truem = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    if (truem)// for debugging reasons
    {
        Vector2 mousePos = GetMousePosition();
        spawnProjectiles(myCharId, myPlayer.posx - myCamera.posx + screenWidth / 2, myPlayer.posy - myCamera.posy + screenHeight / 2, float(mousePos.x), float(mousePos.y), myPlayer);
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
    myChar = chars[myCharId];
    myPlayer.width = myChar.hitbox * 2;
    myPlayer.height = myChar.hitbox * 2;
    myCamera.posx = myPlayer.posx;
    myCamera.posy = myPlayer.posy;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        getKeyboardInput();
        getMouseInput();
        movePlayer();
        moveProjectiles();
        drawScreen();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}