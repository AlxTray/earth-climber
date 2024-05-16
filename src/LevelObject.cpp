#include "LevelObject.h"
#include <raylib.h>

void LevelObject::setBlocks(bool var)
{
	blocks = var;
}

void LevelObject::setBroken(bool var)
{
	broken = var;
	blocks = !blocks;
}

bool LevelObject::getBlocks()
{
	return blocks;
}

bool LevelObject::getAttackOnCollision()
{
	return attackOnCollsion;
}

bool LevelObject::getBroken()
{
	return broken;
}

Color LevelObject::getColor()
{
	return color;
}

// Constructor
MapObject::MapObject(Rectangle b, bool bl, bool br, bool attack, Color cl)
{
	bounds = b;
	blocks = bl;
	broken = br;
	attackOnCollsion = attack;
	color = cl;
}

Rectangle MapObject::getBounds()
{
	return bounds;
}

int MapObject::getBreakCounter()
{
	return breakCounter;
}

void MapObject::incrementBreakCounter()
{
	breakCounter += 1;
}

void MapObject::resetBreakCounter()
{
	breakCounter = 0;
}

void MapObject::draw()
{
	// Draw if not broken, such as brown platforms
	if (!broken) DrawRectangleRec(bounds, color);
}

bool MapObject::checkCollision(Rectangle playerBounds)
{
	return CheckCollisionRecs(playerBounds, bounds);
}

// Constructor
Coin::Coin(Vector2 pos, bool br, Color cl)
{
	position = pos;
	broken = br;
	color = cl;
}

Vector2 Coin::getPosition()
{
	return position;
}

void Coin::draw()
{
	if (!broken) DrawCircleV(position, coinRadius, YELLOW);
}

// Constructor
Flag::Flag(Rectangle flgBnds, Rectangle polBnds)
{
	flagBounds = flgBnds;
	poleBounds = polBnds;
}

bool Flag::checkCollision(Rectangle playerBounds)
{
	return CheckCollisionRecs(playerBounds, poleBounds);
}

void Flag::draw()
{
	DrawRectangleRec(flagBounds, GREEN);
	DrawRectangleRec(poleBounds, BLACK);
}
