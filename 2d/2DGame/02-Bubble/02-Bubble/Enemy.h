#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Character.h"


class Enemy : public Character
{
public:
	void init(int id, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	virtual void update(int deltaTime) = 0;
	void setSpeed(int speed);
	//Stores the lowest value in patrol1 and the highest one in patrol2
	void setPatrolPoints(int point1, int point2);

	int speed;
	int patrol1, patrol2;
	enum {
		A, B
	};

private:
};

#endif _ENEMY_INCLUDE