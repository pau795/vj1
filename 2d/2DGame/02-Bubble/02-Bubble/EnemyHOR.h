#include "Enemy.h"

class EnemyHor :
	public Enemy
{
public:
	void init(int id, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) override;
	void update(int deltaTime) override;
};

