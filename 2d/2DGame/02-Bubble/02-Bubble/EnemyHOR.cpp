#include "EnemyHor.h"

void EnemyHor::update(int deltaTime) {
	sprite->update(deltaTime);
	posCharacter.x += speed;
	if (posCharacter.x >= patrol2) speed *= -1;
	if (posCharacter.x <= patrol1) speed *= -1;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
}
