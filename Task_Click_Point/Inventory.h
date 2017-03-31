#pragma once
#include <vector>
#include <string>
#include "rapidxml.hpp"
#include "entity.h"
#define _CRT_SECURE_NO_WARNINGS

using namespace rapidxml;
using namespace std;

// class for storing data for a dialogue line
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
	string LetterNumber;
	bool owned = false;
};

// class for storing dialogue data
class inventoryTexture
{
public:
	// the name of the dialogue, for indexation
	string name;
	// the lines of the dialogue
	vector<inventoryContent> attr;
	// Get current listed node
	int currentSprite = 0;
};

// function to populate a list of Dialogues from an XML file
vector<inventoryTexture> ReadXmlInventory() {
	FILE *pFile = fopen("Inventory.xml", "rt");
	if (pFile != NULL) {
		fseek(pFile, 0, SEEK_END);
		long docLength = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);
		char *pBuffer = new char[docLength];

		memset(pBuffer, 0, docLength);
		fread(pBuffer, docLength, 1, pFile);
		fclose(pFile);

		xml_document<> doc;
		doc.parse<0>(pBuffer);

		//create temporary list to populate and return
		vector<inventoryTexture> temp;

		// get the main xml node
		xml_node<> *dataNode = doc.first_node();
		// define sprite node
		xml_node<> *sprites = dataNode->first_node("Sprite");
		//for each sprite in the xml
		do {
			// get the type of sprite 
			string spriteType = sprites->first_node("Type")->value();
			bool spriteExists = false;
			// check if that dialogue already exists
			for (int i = 0; i < temp.size(); i++) {
				if (temp[i].name == spriteType)
					spriteExists = true;
			}
			// create character attributes
			inventoryContent c;
			c.spriteName = sprites->first_node("Name")->value();
			c.source = sprites->first_node("Source")->value();
			c.xPos = sprites->first_node("xPos")->value();
			c.yPos = sprites->first_node("yPos")->value();

			// if dialogue already exists, add line to it
			if (spriteExists) {
				temp[temp.size() - 1].attr.push_back(c);
			}
			// else crate a new dialogue and add the line
			else {
				//create a dialogue instance and assign the name
				inventoryTexture s;
				s.name = spriteType;
				s.attr.push_back(c);
				temp.push_back(s);
			}
			sprites = sprites->next_sibling();
		} while (sprites);
		//return populated list
		return temp;
	}
}