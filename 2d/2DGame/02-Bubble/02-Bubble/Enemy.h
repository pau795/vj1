#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Character.h"


class Enemy :
	public Character
{
public:
	virtual void init(int id, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) = 0;
	virtual void update(int deltaTime) = 0;
	int speed;

private:
};

#endif _ENEMY_INCLUDE