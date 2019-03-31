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
	void setPlayGame(bool b);
	bool quitGame();
	void setOptions(vector<string> a);
	void setState(int state);
	int getState();

private:
	void initShaders();

private:
	TexturedQuad *background;
	TexturedQuad *pointer;
	Texture backgroundTexture;
	Texture pointerTexture;
	vector<string> options;
	//state of the menu: 0 - Main Menu, 1 - Instructions, 2 - Credits, 3 - Exit Menu
	int state;
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

