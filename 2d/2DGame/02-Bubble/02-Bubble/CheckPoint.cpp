#include "CheckPoint.h"
#include "Object.h"

enum CheckPointAnims
{
	OFF, ON
};

void CheckPoint::init(int id, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	state = false;
	string file = "data/checkPoint";
	file.append(to_string(id));
	file.append(".txt");
	loadObject(id, file, shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObject.x), float(tileMapDispl.y + posObject.y)));
}
void CheckPoint::update(int deltaTime) {
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObject.x), float(tileMapDispl.y + posObject.y)));
}

int CheckPoint::getPlayerAnimation() {
	return playerAnimation;
}
int CheckPoint::getLevel() {
	return level;
}
int CheckPoint::getGravity() {
	return gravity;
}

bool CheckPoint::isActivated() {
	return state;
}

void CheckPoint::activateCheckPoint(int level, int animation, int gravity) {
	state = true;
	sprite->changeAnimation(ON);
	this->level = level;
	this->playerAnimation = animation;
	this->gravity = gravity;
}

void CheckPoint::desactivateCheckPoint() {
	state = false;
	sprite->changeAnimation(OFF);
}