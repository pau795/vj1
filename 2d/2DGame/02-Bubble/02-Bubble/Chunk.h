#pragma once
#include "Object.h"
class Chunk :
	public Object
{
public:

	bool loadObject(const string &file, ShaderProgram &shaderProgram);
	void init(int id, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void initGravityLine(int id, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
private:

};

