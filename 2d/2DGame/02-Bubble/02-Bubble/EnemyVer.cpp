#include "EnemyVer.h"

void EnemyVer::update(int deltaTime) {
	sprite->update(deltaTime);
	posCharacter.y += speed;
	if (speed > 0) if (posCharacter.y >= patrol2) speed *= -1;
	else if (posCharacter.y <= patrol1) speed *= -1;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
}