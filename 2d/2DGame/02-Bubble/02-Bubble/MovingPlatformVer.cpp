#include "MovingPlatformVer.h"
#include "MovingPlatform.h"

void MovingPlatformVer::update(int deltaTime) {
	if (posObject.y > patrol2)	speed *= -1;
	if (posObject.y < patrol1) speed *= -1;
	posObject.y += speed;
	if (playerLinked && !player->isDead) player->posCharacter.y += speed;
	for (int i = 0; i < chunks.size(); ++i) {
		chunks[i]->posObject.y += speed;
		chunks[i]->sprite->setPosition(glm::vec2(float(chunks[i]->tileMapDispl.x + chunks[i]->posObject.x), float(chunks[i]->tileMapDispl.y + chunks[i]->posObject.y)));
		chunks[i]->sprite->update(deltaTime);
	}

	
}
