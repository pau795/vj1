#ifndef _CHARACTER_INCLUDE
#define _CHARACTER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Character is basically a Sprite that represents a character. As such it has
// all properties it needs to track its movement.


class Character
{

public:
	bool loadCharacter(int id, const string &file, ShaderProgram &shaderProgram);
	virtual void init(int id, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)=0;
	virtual void update(int deltaTime)=0;
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::ivec2 tileMapDispl, posCharacter, characterSize;
	int numAnimations;

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	float offsetX, offsetY;
	
private:
};


#endif // _CHARACTER_INCLUDE


