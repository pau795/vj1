#include "Enemy.h"

void Enemy::init(int id, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram){
	string file ="data/enemy";
	file.append(to_string(id));
	file.append(".txt");
	loadCharacter(file, shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
}

void Enemy::setSpeed(int speed) { 
	this->speed = speed; 
}


void Enemy::setPatrolPoints(int point1, int point2) { 
	if (point1 <= point2){
		this->patrol1 = point1 * map->getTileSize();
		this->patrol2 = point2 * map->getTileSize();
	}
	else {
		this->patrol1 = point2 * map->getTileSize();
		this->patrol2 = point1 * map->getTileSize();
	}
	
}