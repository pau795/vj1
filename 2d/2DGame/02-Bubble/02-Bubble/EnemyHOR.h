#include "Enemy.h"

class EnemyHor :
	public Enemy
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) override;
	void update(int deltaTime) override;
	float speed;
};

