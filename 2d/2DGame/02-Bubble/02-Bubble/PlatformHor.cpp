#include "PlatformHor.h"


void PlatformHor::update(int deltaTime) {
	for (int i = 0; i < chunks.size(); ++i) {
		chunks[i]->sprite->update(deltaTime);
		chunks[i]->sprite->setPosition(glm::vec2(float(chunks[i]->tileMapDispl.x + chunks[i]->posObject.x), float(chunks[i]->tileMapDispl.y + chunks[i]->posObject.y)));
	}
	
}