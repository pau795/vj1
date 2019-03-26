#pragma once
#include "Chunk.h"

class Platform
{
public:
	void init(int id, int size, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void render();
	virtual void update(int deltaTime)=0;
	void setPosition(const glm::vec2 &pos);

	glm::ivec2 posObject, objectSize;
	vector<Chunk*> chunks;
	bool isRight;
private:


};

