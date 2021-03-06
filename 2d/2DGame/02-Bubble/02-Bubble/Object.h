#ifndef _OBJECT_INCLUDE
#define _OBJECT_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

class Object
{
public:
	virtual bool loadObject(const string &file, ShaderProgram &shaderProgram) = 0;
	virtual void init(int id, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) = 0;
	virtual void update(int deltaTime) = 0;
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::ivec2 tileMapDispl, posObject, objectSize;
	int numAnimations;

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	float offsetX, offsetY;

private:

};

#endif //_OBJECT_INCLUDE

