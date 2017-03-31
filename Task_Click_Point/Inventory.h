#pragma once

#include <string>
#include "entity.h"
#define _CRT_SECURE_NO_WARNINGS

using namespace rapidxml;
using namespace std;

// class for storing data for an inventory entry
class inventoryContent{
public:
	// character attributes
	string spriteName;
	string source;
	string xPos;
	string yPos;

	sf::Texture texture;
	sf::Sprite sprite;
	string textureName;
	// string LetterNumber;
	bool owned = false;
};