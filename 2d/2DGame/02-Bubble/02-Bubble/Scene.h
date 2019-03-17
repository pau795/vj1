#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "CheckPoint.h"


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
	bool playerColision(glm::ivec2 p1, glm::ivec2 p2, glm::ivec2 t1, glm::ivec2 t2);

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	vector<Enemy*> enemies;
	vector<CheckPoint*> checkPoints;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int levelId;

};


#endif // _SCENE_INCLUDE

