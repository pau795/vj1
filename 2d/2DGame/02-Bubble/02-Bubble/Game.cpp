#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	initScene = false;
	menu.init();
}

bool Game::update(int deltaTime)
{
	if (menu.playGame()) {
		if (!initScene) {
			scene.init();
			initScene = true;
		}
		scene.update(deltaTime);
		if (scene.gameFinished) {
			menu.setPlayGame(false);
			initScene = false;		
			menu.setOptions({ "Back" });
			menu.setState(2);
		}
	}
	else {
		menu.update(deltaTime);
	}
	if (menu.quitGame()) bPlay = false;
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (menu.playGame() && initScene) {
		scene.render();
	}
	else {
		menu.render();
	}
}

void Game::keyPressed(int key)
{
	if (key == 27) // Escape code
		if (menu.playGame()) {
			menu.setOptions({ "No","Yes" });
			menu.setState(3);
			menu.setPlayGame(false);
		}
		else {
			if (menu.getState() == 0) bPlay = false;
			if (menu.getState() == 1 || menu.getState() == 2) {
				menu.setOptions({ "Play","Instructions", "Credits","Exit" });
				menu.setState(0);
			}
			else if(menu.getState() == 3) menu.setPlayGame(true);
		}
	keys[key] = true;
}

void Game::keyUsed(int key)
{
	keysUsed[key] = true;
}


void Game::keyReleased(int key)
{
	keys[key] = false;
	keysUsed[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyUsed(int key)
{
	specialKeysUsed[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
	specialKeysUsed[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key] && !keysUsed[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key] && !specialKeysUsed[key];
}





