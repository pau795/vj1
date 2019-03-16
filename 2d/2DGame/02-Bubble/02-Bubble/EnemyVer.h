#pragma once
#include "Enemy.h"
class EnemyVer :
	public Enemy
{
public:
	void update(int deltaTime) override;
};

