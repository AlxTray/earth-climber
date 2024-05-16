#pragma once

#include <raylib.h>

// Parent class
class LevelObject
{
public:
	void setBlocks(bool var);
	void setBroken(bool var);
	bool getBlocks();
	bool getBroken();
	bool getAttackOnCollision();
	Color getColor();

protected:
	bool blocks;
	bool broken;
	bool attackOnCollsion;
	Color color;
};

// Class for main platforms
class MapObject : public LevelObject
{
public:
	MapObject(Rectangle b, bool bl, bool br, bool attack, Color cl);

	Rectangle getBounds();
	int getBreakCounter();

	void incrementBreakCounter();
	void resetBreakCounter();
	void draw();
	bool checkCollision(Rectangle playerBounds);

private:
	Rectangle bounds;
	int breakCounter = 0;
};

class Coin : public LevelObject
{
public:
	Coin(Vector2 pos, bool br, Color cl);

	Vector2 getPosition();
	void draw();

private:
	Vector2 position;
	float coinRadius = 35;
};

class Flag : public LevelObject
{
public:
	Rectangle flagBounds;
	Rectangle poleBounds;

	Flag(Rectangle flag, Rectangle pole);

	bool checkCollision(Rectangle playerBounds);
	void draw();
};