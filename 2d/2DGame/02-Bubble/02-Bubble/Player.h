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
	void init(int id, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) override;
	void update(int deltaTime) override;
	void flipGravity();
	int getGravity();
	void setGravity(int gravity);
	bool isDead;
	void changeJumpingSprite();
	void changeLandingSprite();
	void changeDeadSprite();
	bool isJumping();
	void setJumping(bool jump);
	int getLinkedPlatform();
	int getBase();
	void setLinkedPlatform(int pltaform);
	
private:
	bool bJumping;
	int linkedPlatform;
	int fall_step;

	
	

};


#endif  _PLAYER_INCLUDE


