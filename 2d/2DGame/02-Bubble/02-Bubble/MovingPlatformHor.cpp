#include "MovingPlatformHor.h"
#include "MovingPlatform.h"

void MovingPlatformHor::update(int deltaTime) {
	if (posObject.x > patrol2)	speed *= -1;
	if (posObject.x < patrol1) speed *= -1;
	posObject.x += speed;
	if (playerLinked && !player->isDead) player->posCharacter.x += speed;
	for (int i = 0; i < chunks.size(); ++i) {
		chunks[i]->posObject.x += speed;
		chunks[i]->sprite->setPosition(glm::vec2(float(chunks[i]->tileMapDispl.x + chunks[i]->posObject.x), float(chunks[i]->tileMapDispl.y + chunks[i]->posObject.y)));
		chunks[i]->sprite->update(deltaTime);
	}

}