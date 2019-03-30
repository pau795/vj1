#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Text.h"
#include <vector>


#define CAMERA_WIDTH 320
#define CAMERA_HEIGHT 240


class Menu
{

public:
	Menu();
	~Menu();

	void init();
	void update(int deltaTime);
	void render();
	bool playGame();
	bool quitGame();

private:
	void initShaders();

private:
	TexturedQuad *background;
	TexturedQuad *pointer;
	Texture backgroundTexture;
	Texture pointerTexture;
	vector<string> options;
	int selected;
	int choosen;
	bool play;
	bool quit;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Text text;

};


#endif // _MENU_INCLUDE

