#pragma once
#include "Platform.h"
class PlatformHor :
	public Platform
{
public:
	void update(int deltaTime) override;

	int speed;
	int posA, posB;
private:
};

