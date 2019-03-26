#pragma once
#include "Platform.h"
#include "Player.h"
class MovingPlatform :
	public Platform
{
public:
	virtual void update(int deltaTime)=0;
	void setPatrolPoints(int point1, int point2);
	int speed;
	int patrol1, patrol2;
	bool playerLinked;
	Player *player;
private:

};

