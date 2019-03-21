#include "Platform.h"
#include "Chunk.h"
#include <iostream>
#include <fstream>
#include <sstream>

#define CHUNK_SIZE 8;

void Platform::render() {
	for (int i = 0; i < chunks.size(); ++i)
		chunks[i]->render();
}

void Platform::init(int id, int size, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	chunks.clear();
	chunks.resize(size);
	objectSize.x = size * CHUNK_SIZE;
	objectSize.y = CHUNK_SIZE;
	for (int i = 0; i < size; ++i) {
		Chunk * c = new Chunk();
		c->init(id, tileMapPos, shaderProgram);
		chunks[i] = c;
	}
}

void Platform::setPosition(const glm::vec2 &pos)
{
	posObject = pos;
	glm::ivec2 spritePos = pos;
	for (int i = 0; i < chunks.size(); ++i) {
		chunks[i]->setPosition(spritePos);
		chunks[i]->sprite->setPosition(glm::vec2(float(chunks[i]->tileMapDispl.x + chunks[i]->posObject.x), float(chunks[i]->tileMapDispl.y + chunks[i]->posObject.y)));
		spritePos.x += 8;
	}
}
