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
	if(key == 27) // Escape code
		bPlay = false;
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





