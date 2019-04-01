#pragma once
#include "Chunk.h"

class GravityLine
{
public:
	void init(int id, int size, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void init2(int id, int size, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void render();
	void update(int deltaTime);
	void setPosition(const glm::vec2 &pos);
	void setPosition2(const glm::vec2 &pos);

	glm::ivec2 posObject, objectSize;
	vector<Chunk*> chunks;
	bool used = false;
private:


};

