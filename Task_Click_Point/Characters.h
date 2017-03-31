#pragma once

#include <vector>
#include <string>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#define _CRT_SECURE_NO_WARNINGS

using namespace rapidxml;
using namespace std;

// class for storing data for a dialogue line
class characterContent {
public:
	// character attributes
	string charName;
	string spriteLocation;
	string spriteWidth;
	string spriteHeight;
	string xPos;
	string yPos;

};

// class for storing dialogue data
class spriteTexture
{
public:
	// the name of the dialogue, for indexation
	string name;
	// the lines of the dialogue
	vector<characterContent> attr;

	int currentChar = 0;
};

vector <spriteTexture> ReadXmlSprite() {
	rapidxml::file<> xml_file("Characters.xml");
	rapidxml::xml_document<> doc;
	doc.parse<0>(xml_file.data());

	//create temporary list to populate and return
	vector<spriteTexture> temp;

	// get the main xml node
	xml_node<> *dataNode = doc.first_node();
	// define sprite node
	xml_node<> *sprites = dataNode->first_node("Sprite");
	//for each sprite in the xml
	do {
		// get the type of sprite (Character)
		string spriteType = sprites->first_node("spriteType")->value();
		bool spriteExists = false;
		// check if that dialogue already exists
		for (int i = 0; i < temp.size(); i++) {
			if (temp[i].name == spriteType)
				spriteExists = true;
		}
		// create character attributes
		characterContent c;
		c.charName = sprites->first_node("Name")->value();
		c.spriteLocation = sprites->first_node("location")->value();
		c.spriteWidth = sprites->first_node("spriteWidth")->value();
		c.spriteHeight = sprites->first_node("spriteHeight")->value();
		c.xPos = sprites->first_node("xPos")->value();
		c.yPos = sprites->first_node("yPos")->value();

		// if dialogue already exists, add line to it
		if (spriteExists) {
			temp[temp.size() - 1].attr.push_back(c);
		}
		// else crate a new dialogue and add the line
		else {
			//create a dialogue instance and assign the name
			spriteTexture s;
			s.name = spriteType;
			s.attr.push_back(c);
			temp.push_back(s);
		}
		sprites = sprites->next_sibling();
	} while (sprites);

	//return populated list
	return temp;
}