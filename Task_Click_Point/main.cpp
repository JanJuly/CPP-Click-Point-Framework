#define _CRT_SECURE_NO_WARNINGS

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <string>

#include "entity.h"
#include "Player.h"
#include "targets.h"
#include "random.h"

#include <stdio.h>
#include <stdlib.h>
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

class DialogueLine {
public:
	string speaker;
	string text;
};

class Dialogue {
public:
	string name;
	vector<DialogueLine> lines;
	string dialogueContent;
};

vector<Dialogue> ReadXMLNew() {
	FILE *pFile = fopen("Dialogue_Test.xml", "rt");
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
		vector<Dialogue> temp;

		// get the main xml node
		xml_node<> *dataNode = doc.first_node();
		// define DialogueLine nodes
		xml_node<> *dialogueLine = dataNode->first_node("DialogueLine");
		//for each dialogueLine in the xml
		do {
			// get the name of dialogue the line belongs to
			string dialogueName = dialogueLine->first_node("dialogueName")->value();
			bool dialogueExists = false;
			// check if that dialogue already exists
			for (int i = 0; i < temp.size(); i++) {
				if (temp[i].name == dialogueName)
					dialogueExists = true;
			}
			// create line
			DialogueLine l;
			l.speaker = dialogueLine->first_node("speaker")->value();
			l.text = dialogueLine->first_node("line")->value();
			// if dialogue already exists, add line to it
			if (dialogueExists) {
				temp[temp.size() - 1].lines.push_back(l);
			}
			// else crate a new dialogue and add the line
			else {
				//create a dialogue instance and assign the name
				Dialogue d;
				d.name = dialogueName;
				d.lines.push_back(l);
				temp.push_back(d);
			}
			dialogueLine = dialogueLine->next_sibling();
		} while (dialogueLine);
		//return populated list
		return temp;
	}
}

// Plays a given dialogue
void PlayDialogue(Dialogue dialogue) {

	for (int l = 0; l != dialogue.lines.size(); l++)
	{
		dialogue.dialogueContent = dialogue.lines[l].speaker + ": " + dialogue.lines[l].text;
		cout << dialogue.dialogueContent << endl;

	}

}

int main(int, const char**)
{
	int counter = 5;
	sf::RenderWindow window(sf::VideoMode(1000, 600), "Click & Point Programming Task");

	//BACKGROUND SPRITES
	sf::Texture texture_background;
	if (!texture_background.loadFromFile("background.jpg"))
		return EXIT_FAILURE;
	sf::Sprite sprite_background(texture_background);


	vector<Dialogue> dialogues = ReadXMLNew();



	//----------------SETUP CHARACTERS---------------//

		//define textures
	sf::Texture t_1;
	sf::Texture t_2;
	sf::Texture t_3;
	sf::Texture t_4;
	sf::Texture t_5;

	//locate texture files
	t_1.loadFromFile("aladdin.png");
	t_2.loadFromFile("albertsimon.png");
	t_3.loadFromFile("chinese.png");
	t_4.loadFromFile("darion.png");
	t_5.loadFromFile("tseng.png");

	//first character attributes
	class targets char_1;
	char_1.name = "Aladdin";
	char_1.sprite.setTexture(t_1);
	char_1.rect.setPosition(300, 300);
	char_1.width = 32;
	char_1.height = 52;

	//second character attributes
	class targets char_2;
	char_2.name = "Albert";
	char_2.sprite.setTexture(t_2);
	char_2.rect.setPosition(400, 300);
	char_2.width = 32;
	char_2.height = 52;

	//third character attributes
	class targets char_3;
	char_3.name = "Chinese";
	char_3.sprite.setTexture(t_3);
	char_3.rect.setPosition(500, 300);
	char_3.width = 32;
	char_3.height = 48;

	//fourth character attributes
	class targets char_4;
	char_4.name = "Darion";
	char_4.sprite.setTexture(t_4);
	char_4.rect.setPosition(200, 300);
	char_4.width = 32;
	char_4.height = 48;

	//fifth character attributes
	class targets char_5;
	char_5.name = "Tseng";
	char_5.sprite.setTexture(t_5);
	char_5.rect.setPosition(100, 300);
	char_5.width = 32;
	char_5.height = 48;





	//-------------SETUP MOUSE HITBOX----------------//
	class MyMouse {

	public:
		MyMouse(const sf::Window& _window, int mouse_width, int mouse_height) :
			window(_window)
		{
			//initialize box
			mouse_box.width = mouse_width;
			mouse_box.height = mouse_height;
		}

		sf::Vector2i getPosition(const sf::Window &relativeTo) {
			return sf::Mouse::getPosition(relativeTo);
		}

		//WRAP THE REST OF sf::Mouse FUNCTIONS
		bool intersects(sf::FloatRect& other_box) {

			//update before comparing
			mouse_box.left = sf::Mouse::getPosition(window).x;
			mouse_box.top = sf::Mouse::getPosition(window).y;

			return mouse_box.intersects(other_box);
		}

	private:
		const sf::Window& window;
		sf::FloatRect mouse_box;
	};
	MyMouse mouse(window, 10, 10); //initialize mouse hitbox with size of rectangle





//-----------------SETUP  PLAYER----------------//

	sf::Texture texturePlayer;
	if (!texturePlayer.loadFromFile("spriteWalk.png"))
		return EXIT_FAILURE;

	class Player player;
	player.sprite.setTexture(texturePlayer);
	player.sprite.setPosition(window.getSize().x / 2, window.getSize().y / 2 + 200);
	player.sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

	class Dialogue dialogue;





	bool mouseLeftDown = false;
	bool drawText = false;
	string newString;

	// Update function
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !player.isMoving)
			player.mousePosition = sf::Mouse::getPosition(window);

		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left && !mouseLeftDown) {
				//GET CHARACTERS CONTENT
				if (mouse.intersects(char_1.sprite.getGlobalBounds()))
				{
					for (int i = 0; i != dialogues.size(); i++)
						if (dialogues[i].name == "Dialogue_" + char_1.name)
							PlayDialogue(dialogues[i]);

				}
				else if (mouse.intersects(char_2.sprite.getGlobalBounds()))
				{
					for (int i = 0; i != dialogues.size(); i++)
						if (dialogues[i].name == "Dialogue_" + char_2.name)
							PlayDialogue(dialogues[i]);
				}
				else if (mouse.intersects(char_3.sprite.getGlobalBounds()))
				{
					for (int i = 0; i != dialogues.size(); i++)
						if (dialogues[i].name == "Dialogue_" + char_3.name)
							PlayDialogue(dialogues[i]);
				}
				else if (mouse.intersects(char_4.sprite.getGlobalBounds()))
				{
					for (int i = 0; i != dialogues.size(); i++)
						if (dialogues[i].name == "Dialogue_" + char_4.name)
							PlayDialogue(dialogues[i]);
				}
				else if (mouse.intersects(char_5.sprite.getGlobalBounds()))
				{
					for (int i = 0; i != dialogues.size(); i++)
						if (dialogues[i].name == "Dialogue_" + char_5.name)
							PlayDialogue(dialogues[i]);
				}
				mouseLeftDown = true;
			}
		}

		if (event.type == sf::Event::MouseButtonReleased) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				mouseLeftDown = false;
			}
		}



		//----------------UPDATE EACH CHARACTER WALKING CYCLE---------------//
		char_1.walkCycle();
		char_2.walkCycle();
		char_3.walkCycle();
		char_4.walkCycle();
		char_5.walkCycle();

		window.draw(char_1.sprite);
		window.draw(char_2.sprite);
		window.draw(char_3.sprite);
		window.draw(char_4.sprite);
		window.draw(char_5.sprite);


		// ---- MOVE TARGET CHARACTERS AT RANDOM TIME TO RANDOM DIRECTION ----


		//Draw Background
		//window.draw(sprite_background);

		//Draw Text
		//window.draw(text);

		//DRAW PLAYER

		window.draw(player.sprite);
		player.update();
		player.updateMove();



		window.display();

	}


	return 0;
}