#ifndef _CHECK_POINT_INCLUDE
#define _CHECK_POINT_INCLUDE
#include "Object.h"
class CheckPoint :
	public Object
{
public:
	void init(int id, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void activateCheckPoint(int level, int animation, int gravity);
	void desactivateCheckPoint();
	int getPlayerAnimation();
	int getLevel();
	int getGravity();
	bool isActivated();

private:
	bool state;
	int level, playerAnimation, gravity;
};

#endif //_CHECK_POINT_INCLUDE
