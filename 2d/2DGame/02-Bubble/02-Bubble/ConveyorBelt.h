#pragma once
#include "Chunk.h"

class ConveyorBelt
{
public:
	void init(int id, int size, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void render();
	void update(int deltaTime);
	void setPosition(const glm::vec2 &pos);

	glm::ivec2 posObject, objectSize;
	vector<Chunk*> chunks;

private:


};

