#pragma once

#include <vector>
#include <string>
#include <cstring>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#define _CRT_SECURE_NO_WARNINGS

using namespace rapidxml;
using namespace std;

class SpriteContent {
public:
	// name of the sprite
	string name;
	// path to the source file
	string sourceFile;
	// the height of the sprite (not of the file, but of the portion of sprite to draw)
	int spriteHeight;
	// the width of the sprite (not of the file, but of the portion of sprite to draw)
	int spriteWidth;
	// initial x draw position
	int xPos;
	// initial y draw position
	int yPos;
	// eventually the scene in which the sprite should be displayed
	int scene;
};

vector<SpriteContent> ReadXmlSpriteContent(string type) {
	// parse the XML file
	rapidxml::file<> xml_file("Sprites.xml");
	rapidxml::xml_document<> doc;
	doc.parse<0>(xml_file.data());
	//create temporary list to populate and return
	vector<SpriteContent> temp;
	// get the main xml node
	xml_node<> *dataNode = doc.first_node();
	// define sprite node
	xml_node<> *sprites = dataNode->first_node("Sprite");
	//for each sprite in the xml
	do {
		if (sprites->first_node("spriteType")->value() == type) {
			// create character attributes
			SpriteContent c;
			c.name = sprites->first_node("name")->value();
			c.sourceFile = sprites->first_node("location")->value();
			c.spriteWidth = stoi(sprites->first_node("spriteWidth")->value());
			c.spriteHeight = stoi(sprites->first_node("spriteHeight")->value());
			c.xPos = stoi(sprites->first_node("xPos")->value());
			c.yPos = stoi(sprites->first_node("yPos")->value());
			c.scene = stoi(sprites->first_node("scene")->value());
			// push sprite to temporary vector
			temp.push_back(c);
		}
		// move on to next node
		sprites = sprites->next_sibling();
	} while (sprites);
	//return populated list
	return temp;
}