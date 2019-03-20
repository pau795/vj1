#include "Platform.h"
#include <iostream>
#include <fstream>
#include <sstream>

bool Platform::loadObject(const string &file, ShaderProgram &shaderProgram) {


	ifstream fin;
	string line;
	stringstream sstream;

	float spriteSizeTexX = 1.0 / 40, spriteSizeTexY = 1.0 / 30;
	float pixelSizeTexX = spriteSizeTexX / 8, pixelSizeTexY = spriteSizeTexY / 8;
	fin.open(file.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> objectSize.x >> objectSize.y >> numAnimations >> offsetX >> offsetY;
	offsetX *= pixelSizeTexX;
	offsetY *= pixelSizeTexY;
	spritesheet.loadFromFile("images/tiles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(objectSize.x, objectSize.y), glm::vec2(pixelSizeTexX*(objectSize.x), pixelSizeTexY*(objectSize.y)), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(numAnimations);

	for (int i = 0; i < numAnimations; ++i) {
		getline(fin, line);
		sstream.str(line);
		int pos = line.find(" ");
		int n = stoi(line.substr(0, pos));
		string framesDescription = line.substr(pos + 1, line.length() - 1);
		sprite->setAnimationSpeed(i, n);
		string delimiter = "-";
		while (pos != string::npos) {
			pos = framesDescription.find(delimiter);
			string frame = framesDescription.substr(0, pos);
			string frame1 = frame.substr(1, frame.length() - 2);
			int pos1 = frame1.find(",");
			string a = frame1.substr(0, pos1), b = frame1.substr(pos1 + 1, frame1.length() - 1);
			float frameX = stoi(a) * spriteSizeTexX + offsetX, frameY = stoi(b) * spriteSizeTexY + offsetY;
			sprite->addKeyframe(i, glm::vec2(frameX, frameY));
			framesDescription.erase(0, pos + delimiter.length());
		}
	}
	sprite->changeAnimation(0);
}

void Platform::init(int id, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	string file = "data/platform";
	file.append(to_string(id));
	file.append(".txt");
	string fileSpriteSheet = "images/tiles.png";
	loadObject(file, shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObject.x), float(tileMapDispl.y + posObject.y)));
}
