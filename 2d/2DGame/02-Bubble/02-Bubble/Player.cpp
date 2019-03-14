#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "Character.h"

using namespace std;

#define PLAYER_SIZE_X 12
#define PLAYER_SIZE_Y 21
#define KEY_SPACEBAR 32

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, FLIPPED_STAND_LEFT, FLIPPED_STAND_RIGHT, FLIPPED_MOVE_LEFT, FLIPPED_MOVE_RIGHT,
	JUMP_LEFT, JUMP_RIGHT, FLIPPED_JUMP_LEFT, FLIPPED_JUMP_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	fall_step = 4;
	float spriteSizeTexX = 1.0 / 12, spriteSizeTexY = 1.0 / 16;
	float pixelSizeTexX = spriteSizeTexX / 32, pixelSizeTexY = spriteSizeTexY / 32;
	spritesheet.loadFromFile("images/sprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(PLAYER_SIZE_X, PLAYER_SIZE_Y), glm::vec2(pixelSizeTexX*(PLAYER_SIZE_X), pixelSizeTexY*(PLAYER_SIZE_Y+1)), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(12);
	float offsetX = pixelSizeTexX * 6, offsetY = pixelSizeTexY * 2;

		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(3 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 16);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(4 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(5 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 16);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 * spriteSizeTexX +offsetX , 0 * spriteSizeTexY + offsetY));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));

		sprite->setAnimationSpeed(FLIPPED_STAND_LEFT, 8);
		sprite->addKeyframe(FLIPPED_STAND_LEFT, glm::vec2(9 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));

		sprite->setAnimationSpeed(FLIPPED_STAND_RIGHT, 8);
		sprite->addKeyframe(FLIPPED_STAND_RIGHT, glm::vec2(6 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));

		sprite->setAnimationSpeed(FLIPPED_MOVE_LEFT, 16);
		sprite->addKeyframe(FLIPPED_MOVE_LEFT, glm::vec2(9 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));
		sprite->addKeyframe(FLIPPED_MOVE_LEFT, glm::vec2(10 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));
		sprite->addKeyframe(FLIPPED_MOVE_LEFT, glm::vec2(11 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));

		sprite->setAnimationSpeed(FLIPPED_MOVE_RIGHT, 16);
		sprite->addKeyframe(FLIPPED_MOVE_RIGHT, glm::vec2(6 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));
		sprite->addKeyframe(FLIPPED_MOVE_RIGHT, glm::vec2(7 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));
		sprite->addKeyframe(FLIPPED_MOVE_RIGHT, glm::vec2(8 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));

		sprite->setAnimationSpeed(JUMP_LEFT, 8);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(4 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(1 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));

		sprite->setAnimationSpeed(FLIPPED_JUMP_LEFT, 8);
		sprite->addKeyframe(FLIPPED_JUMP_LEFT, glm::vec2(10 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));

		sprite->setAnimationSpeed(FLIPPED_JUMP_RIGHT, 8);
		sprite->addKeyframe(FLIPPED_JUMP_RIGHT, glm::vec2(7 * spriteSizeTexX + offsetX, 0 * spriteSizeTexY + offsetY));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
	
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

void Player::flipGravity() {
	bJumping = true;
	fall_step *= -1;
	changeJumpingSprite();
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	

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
		if (map->collisionMoveLeft(posCharacter, glm::ivec2(PLAYER_SIZE_X, PLAYER_SIZE_Y)))
			posCharacter.x += 2;
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
		if (map->collisionMoveRight(posCharacter, glm::ivec2(PLAYER_SIZE_X, PLAYER_SIZE_Y)))
			posCharacter.x -= 2;
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
		
		if (map->collisionMoveUp(posCharacter, glm::ivec2(PLAYER_SIZE_X, PLAYER_SIZE_Y))) {
			bJumping = false;
			changeLandingSprite();
			posCharacter.y += -fall_step;
			if (Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey(KEY_SPACEBAR)) flipGravity();
		}
		else if (map->collisionMoveDown(posCharacter, glm::ivec2(PLAYER_SIZE_X, PLAYER_SIZE_Y))) {
			bJumping = false;
			posCharacter.y += -fall_step;
			changeLandingSprite();
			if (Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey(KEY_SPACEBAR)) flipGravity();
		}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
}



