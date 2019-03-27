#include <cmath>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "Character.h"

using namespace std;

#define POS_INI_X 4
#define POS_INI_Y 24
#define KEY_SPACEBAR 32

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, 
	FLIPPED_STAND_LEFT, FLIPPED_STAND_RIGHT, FLIPPED_MOVE_LEFT, FLIPPED_MOVE_RIGHT,
	JUMP_LEFT, JUMP_RIGHT, FLIPPED_JUMP_LEFT, FLIPPED_JUMP_RIGHT,
	DEAD_LEFT, DEAD_RIGHT, FLIPPED_DEAD_LEFT, FLIPPED_DEAD_RIGHT
};



void Player::init(int id, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	linkedPlatform = -1;
	isDead = false;
	fall_step = 4;
	loadCharacter("data/player.txt", shaderProgram);	
	tileMapDispl = tileMapPos;
	setPosition(glm::vec2(POS_INI_X * map->getTileSize(), POS_INI_Y * map->getTileSize() - (characterSize.y % map->getTileSize())));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
	sprite->changeAnimation(STAND_RIGHT);
}

void Player::changeJumpingSprite() {
	
	if (sprite->animation() == FLIPPED_MOVE_LEFT || sprite->animation() == FLIPPED_STAND_LEFT)
		sprite->changeAnimation(JUMP_LEFT);
	else if (sprite->animation() == FLIPPED_MOVE_RIGHT || sprite->animation() == FLIPPED_STAND_RIGHT)
		sprite->changeAnimation(JUMP_RIGHT);
	else if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT)
		sprite->changeAnimation(FLIPPED_JUMP_LEFT);
	else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT)
		sprite->changeAnimation(FLIPPED_JUMP_RIGHT);
}
int Player::getBase() {
	if (fall_step > 0) 	return posCharacter.y + characterSize.y;
	else return posCharacter.y;
}


void Player::changeLandingSprite() {
	if (sprite->animation() == JUMP_LEFT)
		sprite->changeAnimation(STAND_LEFT);
	else if (sprite->animation() == JUMP_RIGHT)
		sprite->changeAnimation(STAND_RIGHT);
	else if (sprite->animation() == FLIPPED_JUMP_LEFT)
		sprite->changeAnimation(FLIPPED_STAND_LEFT);
	else if (sprite->animation() == FLIPPED_JUMP_RIGHT)
		sprite->changeAnimation(FLIPPED_STAND_RIGHT);
}

void Player::changeDeadSprite() {
	if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == JUMP_LEFT)
		sprite->changeAnimation(DEAD_LEFT);
	else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == JUMP_RIGHT)
		sprite->changeAnimation(DEAD_RIGHT);
	else if (sprite->animation() == FLIPPED_STAND_LEFT || sprite->animation() == FLIPPED_MOVE_LEFT || sprite->animation() == FLIPPED_JUMP_LEFT)
		sprite->changeAnimation(FLIPPED_DEAD_LEFT);
	else if (sprite->animation() == FLIPPED_STAND_RIGHT || sprite->animation() == FLIPPED_MOVE_RIGHT || sprite->animation() == FLIPPED_JUMP_RIGHT)
		sprite->changeAnimation(FLIPPED_DEAD_RIGHT);
}

void Player::setJumping(bool jump) {
	bJumping = jump;
}

void Player::flipGravity() {
	bJumping = true;
	fall_step *= -1;
	changeJumpingSprite();
}

int Player::getGravity() {
	return fall_step;
}

void Player::setGravity(int gravity) {
	fall_step = gravity;
}

int Player::getLinkedPlatform() {
	return linkedPlatform;
}
void Player::setLinkedPlatform(int platform) {
	linkedPlatform = platform;
}

bool Player::isJumping() {
	return bJumping;
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	
	if (!isDead) {

		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			if (fall_step > 0) {
				if (!bJumping && sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
				else if (bJumping && sprite->animation() != JUMP_LEFT) sprite->changeAnimation(JUMP_LEFT);
			}
			else {
				if (!bJumping && sprite->animation() != FLIPPED_MOVE_LEFT) sprite->changeAnimation(FLIPPED_MOVE_LEFT);
				else if (bJumping && sprite->animation() != FLIPPED_JUMP_LEFT) sprite->changeAnimation(FLIPPED_JUMP_LEFT);
			}
			posCharacter.x -= 2;
			if (map->collisionMoveLeft(posCharacter, glm::ivec2(characterSize.x, characterSize.y))==1)
				posCharacter.x += 2;
			else if (map->collisionMoveLeft(posCharacter, glm::ivec2(characterSize.x, characterSize.y)) == -1) {
				isDead = true;
				changeDeadSprite();
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			if (fall_step > 0) {
				if (!bJumping && sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
				else if (bJumping && sprite->animation() != JUMP_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
			}
			else {
				if (!bJumping && sprite->animation() != FLIPPED_MOVE_RIGHT) sprite->changeAnimation(FLIPPED_MOVE_RIGHT);
				else if (bJumping && sprite->animation() != FLIPPED_JUMP_RIGHT) sprite->changeAnimation(FLIPPED_JUMP_RIGHT);
			}
			posCharacter.x += 2;
			if (map->collisionMoveRight(posCharacter, glm::ivec2(characterSize.x, characterSize.y))==1)
				posCharacter.x -= 2;
			else if (map->collisionMoveRight(posCharacter, glm::ivec2(characterSize.x, characterSize.y)) == -1) {
				isDead = true;
				changeDeadSprite();
			}
		}
		else
		{
			if (sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
			if (sprite->animation() == FLIPPED_MOVE_LEFT)
				sprite->changeAnimation(FLIPPED_STAND_LEFT);
			else if (sprite->animation() == FLIPPED_MOVE_RIGHT)
				sprite->changeAnimation(FLIPPED_STAND_RIGHT);

		}
		bJumping = true;
		posCharacter.y += fall_step;
		if (map->collisionMoveDown(posCharacter, glm::ivec2(characterSize.x, characterSize.y), &posCharacter.y)==1) {
			bJumping = false;
			changeLandingSprite();
			if (Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey(KEY_SPACEBAR)) flipGravity();
		}
		else if (map->collisionMoveDown(posCharacter, glm::ivec2(characterSize.x, characterSize.y), &posCharacter.y) == -1) {
			isDead = true;
			changeDeadSprite();
		}
		else if (map->collisionMoveUp(posCharacter, glm::ivec2(characterSize.x, characterSize.y), &posCharacter.y) == 1) {
			bJumping = false;
			changeLandingSprite();
			if (Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey(KEY_SPACEBAR)) flipGravity();
		}
		else if (map->collisionMoveUp(posCharacter, glm::ivec2(characterSize.x, characterSize.y), &posCharacter.y) == -1) {
			isDead = true;
			changeDeadSprite();
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
}



