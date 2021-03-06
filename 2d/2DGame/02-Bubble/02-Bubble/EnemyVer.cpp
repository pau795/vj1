#include "EnemyVer.h"

void EnemyVer::update(int deltaTime) {
	sprite->update(deltaTime);
	posCharacter.y += speed;
	if (posCharacter.y >= patrol2) {
		speed *= -1;
		sprite->changeAnimation(B);
	}
	if (posCharacter.y <= patrol1) {
		speed *= -1;
		sprite->changeAnimation(A);
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
}