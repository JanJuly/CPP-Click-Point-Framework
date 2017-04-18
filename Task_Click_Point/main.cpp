#define _CRT_SECURE_NO_WARNINGS

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <string>
#include <windows.h>
#include <atlstr.h>

#include "entity.h"
#include "Player.h"
#include "Character.h"
#include "random.h"
#include "Dialogue.h"
#include "Inventory.h"
#include "Sprite.h"
#include "Scene.h"

#include <stdio.h>
#include <stdlib.h>

using namespace std;

// Function to open txt when clicked on letter
void executeFile(string txtFile)
{
	ShellExecuteA(NULL, "open", txtFile.c_str(), NULL, NULL, SW_SHOWNORMAL);
	return;
}

int main(int, const char**)
{

	// Initialize window
	sf::RenderWindow window(sf::VideoMode(856, 480), "Point & Click Programming Task");
	window.setFramerateLimit(60);

	// Initialize mouse hitbox with size of rectangle
	MyMouse mouse(window, 10, 10);

	// Initialize xml files and their class
	vector<Dialogue> dialogues = ReadXmlDialogue(); // includes all dialogue and quest lines
	class Dialogue dialogue;

	// scenes import
	vector<SceneContent> scenesContent = ReadXmlScene();
	vector<Scene> scenes(scenesContent.size());
	for (int s = 0; s < scenes.size(); s++) {
		scenes[s].bgTexture.loadFromFile(scenesContent[s].background);
		sf::Sprite sp(scenes[s].bgTexture);
		scenes[s].bgSprite = sp;
		scenes[s].bgMusic.openFromFile(scenesContent[s].music);
		scenes[s].bgMusic.setVolume(20);
		scenes[s].bgMusic.setLoop(true);
		scenes[s].sceneLeft = scenesContent[s].sceneLeft;
		scenes[s].sceneRight = scenesContent[s].sceneRight;
	}

	// characters import
	vector<SpriteContent> characterSprites = ReadXmlSpriteContent("Character");
	vector<Character> characters(characterSprites.size());
	// convert generic spritecontents into Character
	for (int s = 0; s != characters.size(); s++) {
		characters[s].name = characterSprites[s].name;
		characters[s].height = characterSprites[s].spriteHeight;
		characters[s].width = characterSprites[s].spriteWidth;
		characters[s].sceneNumber = characterSprites[s].scene;
		characters[s].item = characterSprites[s].item;
		characters[s].texture.loadFromFile(characterSprites[s].sourceFile);
		characters[s].sprite.setTexture(characters[s].texture);
		characters[s].rect.setPosition(characterSprites[s].xPos, characterSprites[s].yPos);
	}

	// inventory import
	vector<SpriteContent> inventorySprites = ReadXmlSpriteContent("Letter");
	vector<inventoryContent> items(inventorySprites.size());
	// convert generic spritecontents into inventorycontents
	for (int s = 0; s != items.size(); s++) {
		items[s].textureName = inventorySprites[s].sourceFile;
		items[s].texture.loadFromFile(items[s].textureName);
		items[s].sprite.setTexture(items[s].texture);
		items[s].sprite.setPosition(inventorySprites[s].xPos, inventorySprites[s].yPos);
	}


	//get number of mistakes for guesses
	int mistakes = 0;

	// call entity class for gameplay state
	class entity gameplay;

	// Create text element for speech box
	sf::Font font;
	font.loadFromFile("alterebro-pixel-font.ttf");
	sf::Text speechText("", font, 30);
	speechText.setFillColor(sf::Color::Black);
	sf::String str = "";
	sf::Clock clock;
	int textTimer;
	int textCharacter = 0;

	// Create box element for speech box
	sf::RectangleShape box;
	box.setFillColor(sf::Color::White);
	box.setOutlineThickness(2);
	box.setOutlineColor(sf::Color::Black);

	// Get each character name and class number for current quest
	string characterName = "";
	int characterNumber;
	int currentQuest = 0;

	// setup Inventory template texture
	sf::Texture inventoryTexture;
	inventoryTexture.loadFromFile("Sprites\\inventory.png");
	sf::Sprite inventorySprite;
	inventorySprite.setTexture(inventoryTexture);
	inventorySprite.setPosition(sf::Vector2f(window.getSize().x / 2 - inventorySprite.getLocalBounds().width / 2,
		window.getSize().y / 2 - inventorySprite.getLocalBounds().height / 2));

	//setup sounds
	sf::SoundBuffer key;
	key.loadFromFile("Sounds\\key.ogg");
	sf::Sound keySound;
	keySound.setBuffer(key);
	keySound.setVolume(30);
	keySound.setPitch(1.3);
	keySound.setLoop(true);

	sf::SoundBuffer inventoryBuffer;
	inventoryBuffer.loadFromFile("Sounds\\inventory.ogg");
	sf::Sound inventorySound;
	inventorySound.setBuffer(inventoryBuffer);
	inventorySound.setVolume(30);
	inventorySound.setPitch(1);

	sf::SoundBuffer letterBuffer;
	letterBuffer.loadFromFile("Sounds\\letter.ogg");
	sf::Sound letterSound;
	letterSound.setBuffer(letterBuffer);
	letterSound.setVolume(100);
	letterSound.setPitch(1);




	//-----------------SETUP  PLAYER----------------//
	sf::Texture texturePlayer;
	if (!texturePlayer.loadFromFile("Sprites\\spriteWalk.png"))
		return EXIT_FAILURE;

	class Player player;
	player.sprite.setTexture(texturePlayer);
	player.sprite.setPosition(window.getSize().x / 2 - (player.sprite.getLocalBounds().width), window.getSize().y - 85);
	player.sprite.setScale(3, 3);


	// If key or mousebutton is pressed, return bool true
	// If key or mousebutton is released, return bool false;
	bool mouseLeftDown = false;
	bool keyDown = false;

	// start music background of scene 1
	scenes[0].bgMusic.play();

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


		// Get if left mousekey is in pressed state or released state
		if (event.type == sf::Event::MouseButtonPressed)
			if (event.mouseButton.button == sf::Mouse::Left && !mouseLeftDown)
				mouseLeftDown = true;
		if (event.type == sf::Event::MouseButtonReleased)
			if (event.mouseButton.button == sf::Mouse::Left)
				mouseLeftDown = false;

		// Get if any key is in pressed state or released state
		if (event.type == sf::Event::KeyPressed)
			keyDown = true;
		if (event.type == sf::Event::KeyReleased)
			keyDown = false;



		//----------------DEFINE CURRENT SCENE----------------//
		//if player walks to the right -> get next scene to the right
		if (player.sprite.getPosition().x >= window.getSize().x - 80) {
			// stop old music
			scenes[gameplay.sceneNumber].bgMusic.stop();
			// go to connected scene
			gameplay.sceneNumber = scenes[gameplay.sceneNumber].sceneRight;
			// start new music
			scenes[gameplay.sceneNumber].bgMusic.play();
			// set sprite position to the left of the screen
			player.sprite.setPosition(1, player.sprite.getPosition().y);
		}
		//if player walks to the left -> get next scene to the left
		if (player.sprite.getPosition().x <= window.getSize().x - window.getSize().x) {
			// stop old music
			scenes[gameplay.sceneNumber].bgMusic.stop();
			// go to connected scene
			gameplay.sceneNumber = scenes[gameplay.sceneNumber].sceneLeft;
			// start new music
			scenes[gameplay.sceneNumber].bgMusic.play();
			// set sprite position to the right of the screen
			player.sprite.setPosition(window.getSize().x - 81, player.sprite.getPosition().y);
		}

		// Enable movement of player as soon as the first introduction lines have been shown
		if (!gameplay.introduction)
			player.update();

		// draw current scene background
		if (gameplay.sceneNumber < scenes.size()) {
			window.draw(scenes[gameplay.sceneNumber].bgSprite);
		}



		// draw characters depending on the current scene
		for (int i = 0; i < characters.size(); i++) {
			if (characters[i].sceneNumber == gameplay.sceneNumber)
				window.draw(characters[i].sprite);
		}

		//------------SETUP DIALOGUE SPEECHBOX---------//

		// Draw player above all sprites of scene, but behind dialogue lines
		window.draw(player.sprite);

		//Get width and height of text
		float textWidth = speechText.getLocalBounds().width + 20;
		float textHeight = speechText.getLocalBounds().height + 20;
		//Set Text to pLayers position
		speechText.setPosition(player.sprite.getPosition().x - (textWidth / 2) + 64, player.sprite.getPosition().y - 50);
		//Set Box to text position
		box.setPosition(speechText.getPosition().x - 10, speechText.getPosition().y + 3);
		box.setSize(sf::Vector2f(textWidth, textHeight));
		//Draw speechbox if dialogue is not empty
		if (speechText.getString() != "")
			window.draw(box);
		window.draw(speechText);
		//play key sound of dialogue
		if (textCharacter < str.getSize() && keySound.getStatus() != sf::Sound::Status::Playing)
			keySound.play();
		if (textCharacter >= str.getSize() || gameplay.inventory)
			keySound.stop();

		// Add characters to text with timer
		textTimer = clock.getElapsedTime().asMilliseconds();
		if (textTimer >= 30 && textCharacter != str.getSize() + 1)
		{
			speechText.setString(str.substring(0, textCharacter));
			textCharacter++;
			clock.restart();
		}


		//---------------INVENTORY BEHAVIOUR----------------//

		// enable counter to open/close inventory, prevents of closing and opening too fast
		gameplay.openInventory++;
		if (gameplay.openInventory >= 20 && sf::Keyboard::isKeyPressed(sf::Keyboard::E) && currentQuest >= 1)
		{
			if (!gameplay.inventory)
				gameplay.inventory = true; //open Inventory
			else
				gameplay.inventory = false; //close Inventory

			// if opened/closed reset counter
			gameplay.openInventory = 0;
		}
		if (gameplay.openInventory == 0 && inventorySound.getStatus() != sf::Sound::Status::Playing)
			inventorySound.play();

		// If inventory if open
		if (gameplay.inventory)
		{

			// Draw inventory template background
			window.draw(inventorySprite);
			// Draw Items according to amount of items received
			for (int i = 0; i < items.size(); i++) {
				if (gameplay.numberItems >= i) {
					items[i].owned = true;
					window.draw(items[i].sprite);
				}
			}

			// Open Textfile when clicked on icon
			gameplay.openFile++; // counter for preventing to fast clicking
			if (mouseLeftDown && gameplay.openFile >= 20)
			{
				// For each letter being clicked on, open their text file
				for (int i = 0; i < items.size(); i++) {
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse.intersects(items[i].sprite.getGlobalBounds())) {
						if (items[i].owned) { // if item is drawn and active
							if (letterSound.getStatus() != sf::Sound::Status::Playing)
								letterSound.play();
							gameplay.openFile = 0;
							mouseLeftDown = false;
							string fileName = "Documentation\\letter" + to_string(i + 1) + ".txt";
							executeFile(fileName);
						}
					}
				}
			}
		}




		//-------------------INTRODUCTION DIALOGUE PHASE -------------------//

		//Play Dialogue Phase of Introduction 1
		if (gameplay.introduction)
		{
			// Start with first introdution line
			for (int i = 0; i != dialogues.size(); i++)
				// Look for all dialogue lines named Player
				if (dialogues[i].name == "Introduction")
				{
					str = dialogues[i].lines[dialogues[i].currentLine].text;

					//if scene 2 || scene 3 is entered
					if (gameplay.sceneNumber >= 1 && dialogues[i].currentLine == gameplay.sceneNumber + 1) {
						textCharacter = 0;
						dialogues[i].currentLine++; // show next text
					}
					// enable movement
					if (dialogues[i].currentLine >= 2)
						player.update();

					if (textCharacter == str.getSize() + 1 && !keyDown &&
						sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {

						// show next text in scene 1
						if (dialogues[i].currentLine != 2 && gameplay.sceneNumber == 0 ||
							dialogues[i].currentLine != dialogues[i].lines.size() - 1 && gameplay.sceneNumber == 2) {
							textCharacter = 0;
							dialogues[i].currentLine++;
						}

						// end introduction phase
						if (dialogues[i].currentLine == dialogues[i].lines.size() - 1 && gameplay.sceneNumber == 2) {
							textCharacter = 0;
							currentQuest = 1;
							gameplay.introduction = false;
						}
					}
				}
		}

		//------------------------- QUEST BEHAVIOUR ------------------------//
		//for each quest number get the character belonging to the quest
		for (int i = 1; i < 19; i++)
		{
			if (currentQuest == i) {
				characterName = characters[i - 1].name;
				characterNumber = i - 1;
			}
		}

		// Loop through all quests depending on currentQuest number while inventory is closed
		if (currentQuest != 0 && !gameplay.inventory) {
			string currentCharacter; // save string of current character being looked for
			if (gameplay.firstLine == 0)
				for (int i = 0; i != dialogues.size(); i++) {
					// Look for the dialogueLine name that has the same Name as the character you're looking for
					if (dialogues[i].name == characterName) {
						currentCharacter = dialogues[i].lines[dialogues[i].currentLine].text;
						str = currentCharacter;
						gameplay.firstLine = 1;
					}
				}
			// If player click left mouse button
			if (textCharacter >= str.getSize() && !gameplay.correct) {
				// If player clicks on the correct character sprite
				if (mouse.intersects(characters[characterNumber].sprite.getGlobalBounds()) && mouseLeftDown
					&& characters[characterNumber].sceneNumber == gameplay.sceneNumber  ) {
					str = "You found " + characters[characterNumber].name + " ! ...";
					textCharacter = 0;
					gameplay.correct = true;
				}

				else if (!mouse.intersects(characters[characterNumber].sprite.getGlobalBounds()) && mouseLeftDown) {
					for (int i = 0; i != dialogues.size(); i++)
						if (dialogues[i].name == "Wrong") {
							// display 'wrong' text line
							textCharacter = 0;
							if (textCharacter == 0 && dialogues[i].currentLine != dialogues[i].lines.size() - 1)
								dialogues[i].currentLine++;
							if (textCharacter == 0 && dialogues[i].currentLine == dialogues[i].lines.size() - 1)
							{
								dialogues[i].currentLine = 0;
								gameplay.firstLine = 0;
							}
							str = dialogues[i].lines[dialogues[i].currentLine].text;
						}
				}
			}
			if (gameplay.correct && textCharacter >= str.getSize()) {
				//If character carries a letter
				if (characters[characterNumber].item == 1) {
					if (str != "You have a new letter in your inventory!\nPress [E] ") {
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !keyDown) {
							str = "You have a new letter in your inventory!\nPress [E] ";
							// add item
							gameplay.numberItems = gameplay.numberItems + 1;
							textCharacter = 0;
						}
					}
					else
						if (textCharacter >= str.getSize())
							if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
								// add quest number
								currentQuest = currentQuest + 1;
								textCharacter = 0;
								gameplay.firstLine = 0;
								gameplay.correct = false;
							}
				}
				//If character carries no letter
				else if (characters[characterNumber].item == 0 &&
					sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !keyDown) {
					// add quest number
					currentQuest = currentQuest + 1;
					textCharacter = 0;
					gameplay.firstLine = 0;
					gameplay.correct = false;
				}
			}

		}

		// Update walkcycle of each character existing
		for (int i = 0; i < characters.size(); i++) {
			characters[i].walkCycle();
		}

		// display the newly calculated windoe output
		window.display();

	}
	return 0;
}



