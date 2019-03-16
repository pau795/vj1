#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Character.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player: public Character
{

public:
	bool loadPlayer(int id, const string &file, ShaderProgram &shaderProgram);
	void init(int id, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) override;
	void update(int deltaTime) override;
	
private:
	bool bJumping;
	int fall_step;
	void changeJumpingSprite();
	void changeLandingSprite();
	void flipGravity();

};


#endif  _PLAYER_INCLUDE


