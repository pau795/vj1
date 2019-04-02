#include "Object.h"

#pragma once
class Target :
	public Object
{
public:
	bool loadObject(const string &file, ShaderProgram &shaderProgram);
	void init(int id, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
};

