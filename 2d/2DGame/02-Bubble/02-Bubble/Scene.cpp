#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Scene.h"
#include "Game.h"
#include "Enemy.h"
#include "EnemyVer.h"
#include "EnemyHor.h"


#define SCREEN_X 0
#define SCREEN_Y 0

enum LevelDirections {
	LEFT, RIGHT, TOP, BOTTOM
};

Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}

bool Scene::loadLevel() {
	ifstream fin;
	string line;
	stringstream sstream;

	string file = "levels/data-level";
	file.append(to_string(levelId));
	file.append(".txt");
	fin.open(file.c_str());
	if (!fin.is_open())
		return false;
	//Load level links
	getline(fin, line);
	sstream.str(line);
	int left, right, top, bottom;
	sstream >> left >> right >> top >> bottom;
	levelLinks = { left, right, top, bottom };
	//Load Enemies
	getline(fin, line);
	sstream.str(line);
	int numEnemies;
	sstream >> numEnemies;
	enemies.clear();
	enemies.resize(numEnemies);
	for (int i = 0; i < numEnemies; ++i) {
		getline(fin, line);
		sstream.str(line);
		int id, type, speed, p1x, p1y, p2x, p2y;
		sstream >> id >> type >> speed >> p1x >> p1y >> p2x >> p2y;
		Enemy *e;
		if (type == 0) {
			e = new EnemyHor();
			e->setTileMap(map);
			e->setPatrolPoints(p1x, p2x);
		}
		else {
			e = new EnemyVer();
			e->setTileMap(map);
			e->setPatrolPoints(p1y, p2y);
		}
		e->init(id, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		e->setPosition(glm::vec2(p1x * map->getTileSize(), p1y * map->getTileSize()));
		e->setSpeed(speed);
		enemies[i] = e;
	}

	//Load CheckPoints
	getline(fin, line);
	sstream.str(line);
	int numCheckPoints;
	sstream >> numCheckPoints;
	checkPoints.clear();
	checkPoints.resize(numCheckPoints);
	for (int i = 0; i < numCheckPoints; ++i) {
		getline(fin, line);
		sstream.str(line);
		int orientation, x, y;
		sstream >> orientation >> x >> y;
		CheckPoint* c = new CheckPoint();
		if (currentCheckPoint != NULL && currentCheckPoint->getLevel() == levelId && currentCheckPoint->id == i)
			c = currentCheckPoint;
		else{
			c->init(orientation, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
			c->id = i;
			c->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
		}
		checkPoints[i] = c;
	}
}

void Scene::changeLevel() {
	string file = "levels/level";
	file.append(to_string(levelId));
	file.append(".txt");
	map = TileMap::createTileMap(file, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	loadLevel();
}

void Scene::init()
{
	initShaders();
	levelId = 1;
	deathTimer = -1;
	changeLevel();
	player = new Player();
	player->setTileMap(map);
	player->init(0, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	projection = glm::ortho(0.f, float(320- 1), float(240 - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	//UPDATE ENEMIES
	for (unsigned int i = 0; i < enemies.size(); ++i) {
		enemies[i]->update(deltaTime);
		if (checkColision(player->posCharacter, player->posCharacter + player->characterSize, enemies[i]->posCharacter, enemies[i]->posCharacter + enemies[i]->characterSize)) {
			player->isDead = true;
			player->changeDeadSprite();
		}
	}
	//UPDATE CHECKPOINTS
	for (unsigned int i = 0; i < checkPoints.size(); ++i) {
		if (!checkPoints[i]->isActivated() && checkColision(player->posCharacter, player->posCharacter + player->characterSize, 
			checkPoints[i]->posObject, checkPoints[i]->posObject + checkPoints[i]->objectSize)){
				if (currentCheckPoint != NULL) currentCheckPoint->desactivateCheckPoint();
				checkPoints[i]->activateCheckPoint(levelId, player->sprite->animation(), player->getGravity());
				currentCheckPoint = checkPoints[i];
		}
		checkPoints[i]->update(deltaTime);

	}
	//CHECK RESPAWN CONDITIONS
	if (deathTimer > 0) --deathTimer;
	if (deathTimer == -1 && player->isDead) deathTimer = 30;
	if (deathTimer == 0) {
		player->isDead = false;
		player->setGravity(currentCheckPoint->getGravity());
		if (player->getGravity() > 0) {
			glm::ivec2 a = currentCheckPoint->posObject;
			a.y -= (player->characterSize.y)%map->getTileSize();
			player->setPosition(a);
		}
		else player->setPosition(currentCheckPoint->posObject);
		player->sprite->changeAnimation(currentCheckPoint->getPlayerAnimation());
		if (currentCheckPoint->getLevel() != levelId) {
			levelId = currentCheckPoint->getLevel();
			changeLevel();
			player->setTileMap(map);
		}
		deathTimer = -1;
	}
	//CHECK LEVEL CHANGE
	if (player->posCharacter.x < 0) { //COLLISION WITH LEFT EDGE
		levelId = levelLinks[LEFT];
		changeLevel();
		player->setTileMap(map);
		player->posCharacter.x = map->getNumTilesX()*map->getTileSize() - player->characterSize.x;
	}
	else if (player->posCharacter.x + player->characterSize.x > map->getNumTilesX()*map->getTileSize()) { //COLLISION WITH RIGHT EDGE
		levelId = levelLinks[RIGHT];
		changeLevel();
		player->setTileMap(map);
		player->posCharacter.x = 0;
	}

	else if (player->posCharacter.y < 0) { //COLISION WITH TOP EDGE	
		levelId = levelLinks[TOP];
		changeLevel();
		player->setTileMap(map);
		player->posCharacter.y = map->getNumTilesY()*map->getTileSize() - player->characterSize.y;
	}
	else if (player->posCharacter.y + player->characterSize.y >= map->getNumTilesY()*map->getTileSize()) { //COLLISION WITH BOTTOM EDGE
		levelId = levelLinks[BOTTOM];
		changeLevel();
		player->setTileMap(map);
		player->posCharacter.y = 0;
	}
	
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	for (unsigned int i = 0; i < enemies.size(); ++i) enemies[i]->render();
	for (unsigned int i = 0; i < checkPoints.size(); ++i) checkPoints[i]->render();
	player->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

bool Scene::checkColision(glm::ivec2 p1, glm::ivec2 p2, glm::ivec2 t1, glm::ivec2 t2) {
	
	if (p2.x < t1.x || p1.x > t2.x) return false;
	if (p2.y < t1.y || p1.y > t2.y) return false;

	return true;
}

