#pragma once

#include <raylib.h>

// Player class that handles all movement and updating of the user controllable player character
class Player
{
public:
    Player(Vector2 pos, Rectangle bn, float sp, bool de, bool cnjmp, bool cnlft, bool cnrght);
    Vector2 getPosition();
    void setPosition(int x, int y);
    Rectangle getBounds();
    void updateBounds();
    void resetAccellerator(int xy);
    void setSpeed(float var);
    float getSpeed();
    bool getDead();
    void setDead(bool var);
    bool getCanJump();
    void setCanJump(bool var);
    bool getCanLeft();
    void setCanLeft(bool var);
    bool getCanRight();
    void setCanRight(bool var);

    void draw();
    void moveLeft(bool keyPressed, float delta);
    void moveRight(bool keyPressed, float delta);
    void jump(bool keyPressed, float delta);
    void gravity(float delta);

private:
	Vector2 position;
	Rectangle bounds;
    float speed;
    bool dead;
    bool canJump;
    bool canLeft;
    bool canRight;

    const float MAX_HOR_SPEED = 800.0;
    const float MAX_JUMP_SPEED = 600.0;
    const int GRAVITY = 800; //800

    double xLeftAccelerator = 0.1;
    double xRightAccelerator = 0.1;
    double yAccelerator = 2;
    double yJumpAccelerator = 1;
};