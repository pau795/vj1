#pragma once
#include "Object.h"
class Platform :
	public Object
{
public:
	bool loadObject(const string &file, ShaderProgram &shaderProgram);
	void init(int id, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	virtual void update(int deltaTime)=0;
	int speed;
	int posA, posB;

private:

};

