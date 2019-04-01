#include "GravityLine.h"
#include "Chunk.h"
#include <iostream>
#include <fstream>
#include <sstream>

#define CHUNK_SIZE 8;

void GravityLine::render() {
	for (int i = 0; i < chunks.size(); ++i)
		chunks[i]->render();
}

void GravityLine::init(int id, int size, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	chunks.clear();
	chunks.resize(size);
	objectSize.x = size * CHUNK_SIZE;
	objectSize.y = 1;
	for (int i = 0; i < size; ++i) {
		Chunk * c = new Chunk();
		c->initGravityLine(id, tileMapPos, shaderProgram);
		chunks[i] = c;
	}
}

void GravityLine::init2(int id, int size, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	chunks.clear();
	chunks.resize(size);
	objectSize.x = 1;
	objectSize.y = size * CHUNK_SIZE;
	for (int i = 0; i < size; ++i) {
		Chunk * c = new Chunk();
		c->initGravityLine(id, tileMapPos, shaderProgram);
		chunks[i] = c;
	}
}

void GravityLine::setPosition(const glm::vec2 &pos)
{
	posObject = pos;
	glm::ivec2 spritePos = pos;
	for (int i = 0; i < chunks.size(); ++i) {
		chunks[i]->setPosition(spritePos);
		chunks[i]->sprite->setPosition(glm::vec2(float(chunks[i]->tileMapDispl.x + chunks[i]->posObject.x), float(chunks[i]->tileMapDispl.y + chunks[i]->posObject.y)));
		spritePos.x += 8;
	}
}

void GravityLine::setPosition2(const glm::vec2 &pos)
{
	posObject = pos;
	glm::ivec2 spritePos = pos;
	for (int i = 0; i < chunks.size(); ++i) {
		chunks[i]->setPosition(spritePos);
		chunks[i]->sprite->setPosition(glm::vec2(float(chunks[i]->tileMapDispl.x + chunks[i]->posObject.x), float(chunks[i]->tileMapDispl.y + chunks[i]->posObject.y)));
		spritePos.y += 8;
	}
}

void GravityLine::update(int deltaTime) {
	for (int i = 0; i < chunks.size(); ++i) {
		chunks[i]->sprite->update(deltaTime);
		chunks[i]->sprite->setPosition(glm::vec2(float(chunks[i]->tileMapDispl.x + chunks[i]->posObject.x), float(chunks[i]->tileMapDispl.y + chunks[i]->posObject.y)));
	}

}
