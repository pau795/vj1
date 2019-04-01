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
#include "MovingPlatformHor.h"
#include "MovingPlatformVer.h"
#include "MovingPlatform.h"
#include "ConveyorBelt.h"
#include <GL/glut.h>
#include <irrKlang.h>


#define SCREEN_X 0
#define SCREEN_Y 0
#define KEY_SPACEBAR 32
int movement = 0;

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
	if (map != NULL)
		delete map;
	if (player != NULL)
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
			e->setPatrolPoints(p1x*map->getTileSize(), p2x*map->getTileSize());
		}
		else {
			e = new EnemyVer();
			e->setPatrolPoints(p1y*map->getTileSize(), p2y*map->getTileSize());
		}
		e->setTileMap(map);
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

	//LOAD PLATFORM
	getline(fin, line);
	sstream.str(line);
	int numPlatforms;
	sstream >> numPlatforms;
	platforms.clear();
	platforms.resize(numPlatforms);
	for (int i = 0; i < numPlatforms; ++i) {
		getline(fin, line);
		sstream.str(line);
		int id, type, size, speed,  p1x, p1y, p2x, p2y;
		sstream >> id >> type >> size >>speed >> p1x >> p1y >> p2x >> p2y;
		MovingPlatform* p;
		if (type == 0) {
			p = new MovingPlatformHor();
			p->setPatrolPoints(p1x*map->getTileSize(), p2x*map->getTileSize());
		}
		else {
			p = new MovingPlatformVer();
			p->setPatrolPoints(p1y*map->getTileSize(), p2y*map->getTileSize());
		}
		p->init(id, size, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		p->speed = speed;
		p->player = player;
		p->setPosition(glm::vec2(p1x * map->getTileSize(), p1y * map->getTileSize()));
		platforms[i] = p;
	}

	//LOAD CONVEYOR BELTS
	getline(fin, line);
	sstream.str(line);
	int numConveyorBelts;
	sstream >> numConveyorBelts;
	conveyorBelts.clear();
	conveyorBelts.resize(numConveyorBelts);
	for (int i = 0; i < numConveyorBelts; ++i) {
		getline(fin, line);
		sstream.str(line);
		int id, size, x, y;
		bool auxIsRight;
		sstream >> id >> auxIsRight >> size >> x >> y;
		ConveyorBelt* c = new ConveyorBelt();
		c->init(id, size, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		c->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
		c->isRight = auxIsRight;
		conveyorBelts[i] = c;
	}
	//LOAD GRAVITY LINES HORIZONTAL
	getline(fin, line);
	sstream.str(line);
	int numGravityLines;
	sstream >> numGravityLines;
	gravityLines.clear();
	gravityLines.resize(numGravityLines);
	for (int i = 0; i < numGravityLines; ++i) {
		getline(fin, line);
		sstream.str(line);
		int id, size, x, y;
		sstream >> id >> size >> x >> y;
		GravityLine* gl = new GravityLine();
		gl->init(id, size, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		gl->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
		gravityLines[i] = gl;
	}
	//LOAD GRAVITY LINES VERTICAL
	getline(fin, line);
	sstream.str(line);
	int numGravityLines2;
	sstream >> numGravityLines2;
	gravityLines2.clear();
	gravityLines2.resize(numGravityLines2);
	for (int i = 0; i < numGravityLines2; ++i) {
		getline(fin, line);
		sstream.str(line);
		int id, size, x, y;
		sstream >> id >> size >> x >> y;
		GravityLine* gl = new GravityLine();
		gl->init2(id, size, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		gl->setPosition2(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
		gravityLines2[i] = gl;
	}
}

void Scene::changeLevel() {
	string file = "levels/level";
	file.append(to_string(levelId));
	file.append(".txt");
	map = TileMap::createTileMap(file, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	loadLevel();
	if (!soundEngine->isCurrentlyPlaying("sounds/music1.ogg")) soundEngine->play2D("sounds/music1.ogg", true);
}

void Scene::init()
{
	initShaders();
	levelId = 1;
	deathTimer = -1;
	soundEngine = irrklang::createIrrKlangDevice();
	changeLevel();
	player = new Player();
	player->setTileMap(map);
	player->init(0, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->soundEngine = soundEngine;
	for(unsigned int i =0; i< platforms.size(); ++i) platforms[i]->player = player;
	projection = glm::ortho(0.f, float(320 - 1), float(240 - 1), 0.f);
	currentTime = 0.0f;

	backgroundTexture.loadFromFile("images/scene-background.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexture.setMagFilter(GL_NEAREST);
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	//UPDATE ENEMIES
	for (unsigned int i = 0; i < enemies.size(); ++i) {
		enemies[i]->update(deltaTime);
		if (!player->isDead && checkColision(player->posCharacter, player->posCharacter + player->characterSize, enemies[i]->posCharacter, enemies[i]->posCharacter + enemies[i]->characterSize)) {
			soundEngine->play2D("sounds/hurt.ogg");
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
			soundEngine->play2D("sounds/save.ogg");
			currentCheckPoint = checkPoints[i];
		}
		checkPoints[i]->update(deltaTime);
	}
	//UPDATE MOVING PLATFORMS
	bool colision = false;
	for (unsigned int i = 0; i < platforms.size(); ++i) {
		if (checkColisionPlatform(player->posCharacter.x, player->posCharacter.x + player->characterSize.x, player->getBase() + player->getGravity(), platforms[i]->posObject, platforms[i]->posObject + platforms[i]->objectSize)) {
			player->setJumping(false);
			player->changeLandingSprite();
			int gravity = player->getGravity();
			if (gravity > 0 && !player->isDead) player->posCharacter.y = platforms[i]->posObject.y-player->characterSize.y-gravity;
			else  if (!player->isDead) player->posCharacter.y = platforms[i]->posObject.y+ platforms[i]->objectSize.y-gravity;
			player->setLinkedPlatform(i);
			colision = true;
			platforms[i]->playerLinked = true;
			if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
				Game::instance().specialKeyUsed(GLUT_KEY_UP);
				player->flipGravity();
			}
			else if (Game::instance().getKey(KEY_SPACEBAR)) {
				Game::instance().keyUsed(KEY_SPACEBAR);
				player->flipGravity();
			}
		}
		if (!colision && player->getLinkedPlatform() >= 0) {
			platforms[player->getLinkedPlatform()]->playerLinked = false;
			player->setLinkedPlatform(-1);
		}
		platforms[i]->update(deltaTime);
	}
	//UPDATE CONVEYER BELT
	for (unsigned int i = 0; i < conveyorBelts.size(); ++i) {
		conveyorBelts[i]->update(deltaTime);
		if (checkColision(player->posCharacter, player->posCharacter + player->characterSize, conveyorBelts[i]->posObject, conveyorBelts[i]->posObject + conveyorBelts[i]->objectSize)) {
			int drag;
			if (conveyorBelts[i]->isRight) drag= deltaTime / 10;
			else drag = -deltaTime / 10;
			player->posCharacter.x += drag;
			if (map->collisionMoveLeft(player->posCharacter, glm::ivec2(player->characterSize.x, player->characterSize.y)) == 1)
				player->posCharacter.x -= drag;
			else if (map->collisionMoveLeft(player->posCharacter, glm::ivec2(player->characterSize.x, player->characterSize.y)) == -1) {
				player->isDead = true;
				player->changeDeadSprite();
			}
			else if (map->collisionMoveRight(player->posCharacter, glm::ivec2(player->characterSize.x, player->characterSize.y)) == 1)
				player->posCharacter.x -= drag;
			else if (map->collisionMoveRight(player->posCharacter, glm::ivec2(player->characterSize.x, player->characterSize.y)) == -1) {
				player->isDead = true;
				player->changeDeadSprite();
			}
			else if (player->isDead) player->posCharacter.x -= drag;
		}
	}
	//CHECK RESPAWN CONDITIONS
	if (deathTimer > 0) --deathTimer;
	if (deathTimer == -1 && player->isDead) deathTimer = 40;
	if (deathTimer == 0) {
		player->isDead = false;
		player->setGravity(currentCheckPoint->getGravity());
		if (player->getGravity() > 0) {
			glm::ivec2 a = currentCheckPoint->posObject;
			a.y -= (player->characterSize.y) % map->getTileSize();
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
		player->setLinkedPlatform(-1);
		player->setTileMap(map);
		player->posCharacter.x = map->getNumTilesX()*map->getTileSize() - player->characterSize.x;
	}
	else if (player->posCharacter.x + player->characterSize.x >= map->getNumTilesX()*map->getTileSize()) { //COLLISION WITH RIGHT EDGE
		levelId = levelLinks[RIGHT];
		changeLevel();
		player->setLinkedPlatform(-1);
		player->setTileMap(map);
		player->posCharacter.x = 0;
	}

	else if (player->posCharacter.y < 0) { //COLISION WITH TOP EDGE	
		levelId = levelLinks[TOP];
		changeLevel();
		player->setLinkedPlatform(-1);
		player->setTileMap(map);
		player->posCharacter.y = map->getNumTilesY()*map->getTileSize() - player->characterSize.y;
	}
	else if (player->posCharacter.y + player->characterSize.y >= map->getNumTilesY()*map->getTileSize()) { //COLLISION WITH BOTTOM EDGE
		levelId = levelLinks[BOTTOM];
		changeLevel();
		player->setLinkedPlatform(-1);
		player->setTileMap(map);
		player->posCharacter.y = 0;
	}
	player->update(deltaTime);

	//CHECK GRAVITY LINE HOR
	for (unsigned int i = 0; i < gravityLines.size(); ++i) {
		gravityLines[i]->update(deltaTime);
		int n = player->getGravity();
		if (n <= 0 && !gravityLines[i]->used) {
			if (checkColision(player->posCharacter, player->posCharacter + player->characterSize, gravityLines[i]->posObject, gravityLines[i]->posObject + gravityLines[i]->objectSize)) {
				player->changeLandingSprite();
				player->flipGravity();
				gravityLines[i]->used = true;
			}
		}
		else if (n >= 0 && !gravityLines[i]->used) {
			if (checkColision(player->posCharacter, player->posCharacter + player->characterSize, gravityLines[i]->posObject, gravityLines[i]->posObject + gravityLines[i]->objectSize)) {
				player->changeLandingSprite();
				player->flipGravity();
				gravityLines[i]->used = true;
			}
		}
		if (checkColisionGravityLine(player->posCharacter, player->posCharacter + player->characterSize, gravityLines[i]->posObject, gravityLines[i]->posObject + gravityLines[i]->objectSize)) {
			gravityLines[i]->used = false;
		}
	}
	//CHECK GRAVITY LINE VER
	for (unsigned int i = 0; i < gravityLines2.size(); ++i) {
		gravityLines2[i]->update(deltaTime);
		int n = player->getGravity();
		if (n <= 0 && !gravityLines2[i]->used) {
			if (checkColision(player->posCharacter, player->posCharacter + player->characterSize, gravityLines2[i]->posObject, gravityLines2[i]->posObject + gravityLines2[i]->objectSize)) {
				player->changeLandingSprite();
				player->flipGravity();
				gravityLines2[i]->used = true;
			}
		}
		else if (n >= 0 && !gravityLines2[i]->used) {
			if (checkColision(player->posCharacter, player->posCharacter + player->characterSize, gravityLines2[i]->posObject, gravityLines2[i]->posObject + gravityLines2[i]->objectSize)) {
				player->changeLandingSprite();
				player->flipGravity();
				gravityLines2[i]->used = true;
			}
		}
		if (checkColisionGravityLine(player->posCharacter, player->posCharacter + player->characterSize, gravityLines2[i]->posObject, gravityLines2[i]->posObject + gravityLines2[i]->objectSize)) {
			gravityLines2[i]->used = false;
		}
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
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(320), float(240)) };
	glm::vec2 texCoords[2] = { glm::vec2(0+(movement / 1000.0), 0.f), glm::vec2(float(1+(movement/1000.0)), 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	background->render(backgroundTexture);
	movement += 1%1000;
	map->render();
	for (unsigned int i = 0; i < enemies.size(); ++i) enemies[i]->render();
	for (unsigned int i = 0; i < checkPoints.size(); ++i) checkPoints[i]->render();
	for (unsigned int i = 0; i < platforms.size(); ++i) platforms[i]->render();
	for (unsigned int i = 0; i < conveyorBelts.size(); ++i) conveyorBelts[i]->render();
	for (unsigned int i = 0; i < gravityLines.size(); ++i) gravityLines[i]->render();
	for (unsigned int i = 0; i < gravityLines2.size(); ++i) gravityLines2[i]->render();
	player->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
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

bool Scene::checkColisionGravityLine(glm::ivec2 p1, glm::ivec2 p2, glm::ivec2 t1, glm::ivec2 t2) {

	if (p2.x < t1.x || p1.x > t2.x) return true;
	if (p2.y < t1.y || p1.y > t2.y) return true;

	return false;
}

bool Scene::checkColisionPlatform(int x1, int x2, int y, glm::ivec2 t1, glm::ivec2 t2) {

	if ((x1 > t1.x || x2 > t1.x) && (x1 < t2.x || x2 < t2.x) && y >t1.y && y < t2.y) return true;

	return false;
}