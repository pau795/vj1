#pragma once
#include "Platform.h"
class PlatformVer :
	public Platform
{
public:
	void update(int deltaTime) override;

	int speed;
	int posA, posB;
private:

};

