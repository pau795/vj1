#pragma once
#include "Chunk.h"
#include "Platform.h"

class ConveyorBelt :
	public Platform
{
public:
	void update(int deltaTime) override;
	bool isRight;
private:


};

