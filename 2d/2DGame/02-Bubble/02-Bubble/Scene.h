#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "MovingPlatform.h"
#include "ConveyorBelt.h"
#include "CheckPoint.h"
#include <irrKlang.h>
#include "TexturedQuad.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();
	bool loadLevel();
	void init();
	void update(int deltaTime);
	void render();
	bool checkColision(glm::ivec2 p1, glm::ivec2 p2, glm::ivec2 t1, glm::ivec2 t2);
	bool checkColisionPlatform(int x1, int x2, int y, glm::ivec2 t1, glm::ivec2 t2);
	void changeLevel();
	irrklang::ISoundEngine* soundEngine;

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	vector<Enemy*> enemies;
	vector<CheckPoint*> checkPoints;
	vector<MovingPlatform*> platforms;
	vector<ConveyorBelt*> conveyorBelts;
	CheckPoint *currentCheckPoint;
	int levelId;
	int deathTimer;
	vector<int> levelLinks;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	TexturedQuad *background;
	Texture backgroundTexture;

};


#endif // _SCENE_INCLUDE

