#include "Object.h"
#include <iostream>
#include <fstream>
#include <sstream>


void Object::render()
{
	sprite->render();
}

void Object::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Object::setPosition(const glm::vec2 &pos)
{
	posObject = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObject.x), float(tileMapDispl.y + posObject.y)));
}
