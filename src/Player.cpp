#include "Player.h"
#include <raylib.h>

// Constructor
Player::Player(Vector2 pos, Rectangle bn, float sp, bool de, bool cnjmp, bool cnlft, bool cnrght)
{
	position = pos;
	bounds = bn;
	speed = sp;
	dead = de;
	canJump = cnjmp;
	canLeft = cnlft;
	canRight = cnrght;
}

Vector2 Player::getPosition()
{
	return position;
}

void Player::setPosition(int x, int y)
{
	position.x = x;
	position.y = y;
}

Rectangle Player::getBounds()
{
	return bounds;
}

void Player::updateBounds()
{
	bounds = { position.x, position.y, 80, 80 };
}

// Resets the accelrator values of the player, selection of both x = 1, y = 2, both x and y = 3
void Player::resetAccellerator(int xy)
{
	if (xy == 0)
	{
		xLeftAccelerator = 0.1;
		xRightAccelerator = 0.1;
	}
	else if (xy == 1) yAccelerator = 0;

	else
	{
		xLeftAccelerator = 0.1;
		xRightAccelerator = 0.1;
		yAccelerator = 0;
	}
}

// Set vertical speed
void Player::setSpeed(float var)
{
	speed = var;
}

float Player::getSpeed()
{
	return speed;
}

bool Player::getDead()
{
	return dead;
}

void Player::setDead(bool var)
{
	dead = var;
}

bool Player::getCanJump()
{
	return canJump;
}

void Player::setCanJump(bool var)
{
	canJump = var;
}

bool Player::getCanLeft()
{
	return canLeft;
}

void Player::setCanLeft(bool var)
{
	canLeft = var;
}

bool Player::getCanRight()
{
	return canRight;
}

void Player::setCanRight(bool var)
{
	canRight = var;
}


// Draws player using bounds
void Player::draw()
{
	DrawRectangleRec(bounds, BLACK);
}

void Player::moveLeft(bool keyPressed, float delta)
{
	if (keyPressed && canLeft)
	{
		position.x -= (MAX_HOR_SPEED * xLeftAccelerator) * delta;
		if (xLeftAccelerator < 1) xLeftAccelerator += 0.01;
	}
	else
	{
		if (xLeftAccelerator > 0.1)
		{
			position.x -= (MAX_HOR_SPEED * xLeftAccelerator) * delta;
			xLeftAccelerator -= 0.03;
		}
		else
		{
			xLeftAccelerator = 0.1;
		}
	}
}

void Player::moveRight(bool keyPressed, float delta)
{
	if (keyPressed && canRight)
	{
		position.x += (MAX_HOR_SPEED * xRightAccelerator) * delta;
		//Increases acceleration of square in the current direction
		if (xRightAccelerator < 1) xRightAccelerator += 0.01;
	}
	else
	{
		if (xRightAccelerator > 0.1)
		{
			position.x += (MAX_HOR_SPEED * xRightAccelerator) * delta;
			xRightAccelerator -= 0.03;
		}
		else
		{
			xRightAccelerator = 0.1;
		}
	}
}

void Player::jump(bool keyPressed, float delta)
{
	if (keyPressed && canJump && yJumpAccelerator < 1.25)
	{
		speed = -(MAX_JUMP_SPEED * yJumpAccelerator);
		yJumpAccelerator += 0.01;
	}
	else
	{
		canJump = false;
		yJumpAccelerator = 1;
	}
}

void Player::gravity(float delta)
{
	position.y += speed * delta;
	speed += (GRAVITY * yAccelerator) * delta;
	if (yAccelerator != 4) yAccelerator += 0.2;
}
