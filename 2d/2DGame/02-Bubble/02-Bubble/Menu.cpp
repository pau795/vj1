#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"
#include "Game.h"
#include <GL/glut.h>


#define KEY_SPACEBAR 32
#define KEY_RETURN 13

Menu::Menu()
{
	background = NULL;
}

Menu::~Menu()
{
	if (background != NULL) delete background;
}


void Menu::init()
{
	initShaders();
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(320), float(240)) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	backgroundTexture.loadFromFile("images/menu-background.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexture.setMagFilter(GL_NEAREST);
	glm::vec2 geom1[2] = { glm::vec2(0.f, 0.f), glm::vec2(8.f, 8.f) };
	glm::vec2 texCoords1[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	pointer = TexturedQuad::createTexturedQuad(geom1, texCoords1, texProgram);
	pointerTexture.loadFromFile("images/pointer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	pointerTexture.setMagFilter(GL_NEAREST);
	projection = glm::ortho(0.f, float(320 - 1), float(240 - 1), 0.f);
	currentTime = 0.0f;
	selected = 0;
	choosen = -1;
	state = 0;
	play = false;
	text.init("fonts/OpenSans-Bold.ttf");
	options = { "Play","Instructions", "Credits","Exit"};
}

bool Menu::playGame() {
	return play;
}

void Menu::update(int deltaTime)
{
	currentTime += deltaTime;
	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		Game::instance().specialKeyUsed(GLUT_KEY_DOWN);
		selected = (selected +1)% options.size();
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		Game::instance().specialKeyUsed(GLUT_KEY_UP);
		--selected;
		if (selected < 0) selected += options.size();
	}
	else if (Game::instance().getKey(KEY_RETURN)) {
		Game::instance().keyUsed(KEY_RETURN);
		choosen = selected;
	}
	else if (Game::instance().getKey(KEY_SPACEBAR)) {
		Game::instance().keyUsed(KEY_SPACEBAR);
		choosen = selected;
	}
	switch (state) {
		case 0:		//MAIN MENU
			switch (choosen){
				case 0:
					choosen = -1;
					selected = 0;
					play = true;
					break;
				case 1:
					choosen = -1;
					options = { "Back" };
					selected = 0;
					state = 1;
					break;
				case 2:
					choosen = -1;
					selected = 0;
					options = { "Back" };
					state = 2;
					break;
				case 3:
					quit = true;
					break;
			}
		case 1:	//INSTRUCTIONS
			if (choosen == 0) {
				choosen = -1;
				selected = 0;
				state = 0;
				options = { "Play","Instructions", "Credits","Exit" };
				break;
			}
			break;
		case 2: // CREDITS
			if (choosen == 0) {
				choosen = -1;
				selected = 0;
				state = 0;
				options = { "Play","Instructions", "Credits","Exit" };
				break;
			}
			break;
		case 3:	//EXIT MENU
			if (choosen == 0) {
				choosen = -1;
				selected = 0;
				state = 0;
				play = true;
				break;
			}
			else if (choosen == 1) {
				quit = true;
				break;
			}
			break;
	}
}

bool Menu::quitGame() {
	return quit;
}

void Menu::render()
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	background->render(backgroundTexture);
	glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);
	switch (state) {
		case 0:			//MAIN MENU
			modelview = glm::translate(glm::mat4(1.0f), glm::vec3(15.f, float(90 + 20 * selected), 0.f));
			texProgram.setUniformMatrix4f("modelview", modelview);
			pointer->render(pointerTexture);
			color = glm::vec4(1.f, 1.f, 1.f, 1.f);
			text.render("Main Menu", glm::vec2(40, 150), 32, glm::vec4(0.2, 0.8, 0.2, 1));
			for (unsigned int i = 0; i < options.size(); ++i) {
				glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);
				if (i == selected) color = glm::vec4(0.f, 0.f, 1.f, 1.f);
				text.render(options[i], glm::vec2(60, float(200 + 40 * i)), 24, color);
			}
			break;
		case 1:		//INSTRUCTIONS
			modelview = glm::translate(glm::mat4(1.0f), glm::vec3(15.f, float(120 + 20 * selected), 0.f));
			texProgram.setUniformMatrix4f("modelview", modelview);
			pointer->render(pointerTexture);
			color = glm::vec4(1.f, 1.f, 1.f, 1.f);
			text.render("Instructions", glm::vec2(40, 50), 32, glm::vec4(0.2, 0.8, 0.2, 1));
			text.render("Use left and right keys to move around.", glm::vec2(50, 100), 20, glm::vec4(1, 1, 1, 1));
			text.render("Use up key or spacebar to flip gravity.", glm::vec2(50, 140), 20, glm::vec4(1, 1, 1, 1));
			text.render("Avoid all the enemies and obstacles.", glm::vec2(50, 180), 20, glm::vec4(1, 1, 1, 1));
			text.render("Explore the world and advance through the levels!", glm::vec2(50, 220), 20, glm::vec4(1, 1, 1, 1));
			for (unsigned int i = 0; i < options.size(); ++i) {
				glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);
				if (i == selected) color = glm::vec4(0.f, 0.f, 1.f, 1.f);
				text.render(options[i], glm::vec2(60, float(260 + 40 * i)), 24, color);
			}
			break;
		case 2:		//CREDITS
			modelview = glm::translate(glm::mat4(1.0f), glm::vec3(15.f, float(140 + 20 * selected), 0.f));
			texProgram.setUniformMatrix4f("modelview", modelview);
			pointer->render(pointerTexture);
			color = glm::vec4(1.f, 1.f, 1.f, 1.f);
			text.render("Credits", glm::vec2(40, 50), 32, glm::vec4(0.2, 0.8, 0.2, 1));
			text.render("Game created by:", glm::vec2(50, 100), 20, glm::vec4(1, 1, 1, 1));
			text.render("Pablo Navarro", glm::vec2(80, 140), 20, glm::vec4(1, 1, 1, 1));
			text.render("Ferran Vallès", glm::vec2(80, 180), 20, glm::vec4(1, 1, 1, 1));
			text.render("Videogames Project, Spring 2019.", glm::vec2(50, 220), 20, glm::vec4(1, 1, 1, 1));
			text.render("FIB, UPC.", glm::vec2(50, 260), 20, glm::vec4(1, 1, 1, 1));
			for (unsigned int i = 0; i < options.size(); ++i) {
				glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);
				if (i == selected) color = glm::vec4(0.f, 0.f, 1.f, 1.f);
				text.render(options[i], glm::vec2(60, float(300 + 40 * i)), 24, color);
			}
			break;
		case 3:		//EXIT MENU
			modelview = glm::translate(glm::mat4(1.0f), glm::vec3(15.f, float(90 + 20 * selected), 0.f));
			texProgram.setUniformMatrix4f("modelview", modelview);
			pointer->render(pointerTexture);
			color = glm::vec4(1.f, 1.f, 1.f, 1.f);
			text.render("Are you sure you want to exit?", glm::vec2(40, 150), 32, glm::vec4(0.2, 0.8, 0.2, 1));
			for (unsigned int i = 0; i < options.size(); ++i) {
				glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);
				if (i == selected) color = glm::vec4(0.f, 0.f, 1.f, 1.f);
				text.render(options[i], glm::vec2(60, float(200 + 40 * i)), 24, color);
			}
			break;
	}
	

}

void Menu::setPlayGame(bool b) {
	play = b;
}

void Menu::setOptions(vector<string> a) {
	options = a;
}
void Menu::setState(int state) {
	this->state = state;
	choosen = -1;
	selected = 0;
}
int Menu::getState() {
	return state;
}

void Menu::initShaders()
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
}

