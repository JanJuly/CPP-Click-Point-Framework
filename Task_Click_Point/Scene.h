#pragma once

#include <vector>
#include <string>
#include <cstring>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#define _CRT_SECURE_NO_WARNINGS

using namespace rapidxml;
using namespace std;

class SceneContent {
public:
	// path to the background image file
	string background;
	// path to the music file
	string music;
	// scene connected to the left
	int sceneLeft;
	// scene connected to the right
	int sceneRight;
};

class Scene {
public:
	sf::Texture bgTexture;
	sf::Sprite bgSprite;
	sf::Music bgMusic;
	int sceneLeft;
	int sceneRight;
};

vector<SceneContent> ReadXmlScene() {
	// parse the XML file
	rapidxml::file<> xml_file("Scenes.xml");
	rapidxml::xml_document<> doc;
	doc.parse<0>(xml_file.data());
	//create temporary list to populate and return
	vector<SceneContent> temp;
	// get the main xml node
	xml_node<> *dataNode = doc.first_node();
	// define sprite node
	xml_node<> *sprites = dataNode->first_node("Scene");
	//for each sprite in the xml
	do {
		// create character attributes
		SceneContent c;
		c.background = sprites->first_node("background")->value();
		c.music = sprites->first_node("music")->value();
		c.sceneLeft = stoi(sprites->first_node("connectedLeft")->value());
		c.sceneRight = stoi(sprites->first_node("connectedRight")->value());
		// push sprite to temporary vector
		temp.push_back(c);
		// move on to next node
		sprites = sprites->next_sibling();
	} while (sprites);
	//return populated list
	return temp;
}