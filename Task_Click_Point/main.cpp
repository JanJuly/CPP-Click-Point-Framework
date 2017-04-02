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

void playMusic(string musicSource)
{

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

	// define dialogue string and text character amount
	sf::String str = "";
	int amountTextChar = 0;

	//get number of mistakes for guesses
	int mistakes = 0;

	// call entity class for gameplay state
	class entity gameplay;

	// Create text element for speech box
	sf::Font font;
	font.loadFromFile("alterebro-pixel-font.ttf");
	sf::Text speechText("", font, 30);
	speechText.setFillColor(sf::Color::Black);
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
		textTimer = clock.getElapsedTime().asMilliseconds();

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
		if (gameplay.introductionNumber != 1)
			player.update();

		// draw current scene background
		if (gameplay.sceneNumber < scenes.size()) {
			window.draw(scenes[gameplay.sceneNumber].bgSprite);
		}


		//-------------------COUNT QUEST STEPS-------------------//
		if (gameplay.sceneNumber == 1) {
			if (gameplay.introductionNumber == 1) {
				gameplay.introductionNumber = 2;
				clock.restart();
				textCharacter = 0;
			}
		}

		if (gameplay.sceneNumber == 2) {
			if (gameplay.introductionNumber == 2) {
				gameplay.introductionNumber = 3;
				clock.restart();
				textCharacter = 0;
			}
		}

		// draw characters dpending on the current scene
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
		if (textCharacter < amountTextChar && keySound.getStatus() != sf::Sound::Status::Playing)
			keySound.play();
		if (textCharacter >= amountTextChar || gameplay.inventory)
			keySound.stop();


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
			gameplay.wrong = false;
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




		//-------------------INTRODUCTION 1-3 -------------------//

		//Play Dialogue Phase of Introduction 1
		if (gameplay.introductionNumber == 1)
		{
			// Start with first introdution line
			for (int i = 0; i != dialogues.size(); i++)
				// Look for all dialogue lines named Player
				if (dialogues[i].name == "Introduction")
				{
					// Setup text character animation
					if (textTimer >= 30 && textCharacter < dialogues[i].lines[dialogues[i].currentLine].text.length())
					{
						textCharacter++;
						str = dialogues[i].lines[dialogues[i].currentLine].text;
						speechText.setString(str.substring(0, textCharacter));
						clock.restart();
					}

					// Show next line while pressing Enter
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !keyDown &&
						textCharacter == dialogues[i].lines[dialogues[i].currentLine].text.length()
						&& dialogues[i].currentLine != 2)
					{
						textCharacter = 0;
						dialogues[i].currentLine++;
					}
					// If all lines have been shown, introduction sequence is over
					if (textCharacter == dialogues[i].lines[dialogues[i].currentLine].text.length()
						&& dialogues[i].currentLine == 2)
					{
						player.update();
					}
				}
			amountTextChar = str.getSize();
		}
		//Play Dialogue Phase of Introduction 2
		if (gameplay.introductionNumber == 2)
		{
			// Start with first introdution line
			for (int i = 0; i != dialogues.size(); i++)
			{
				// Look for all dialogue lines named Player
				if (textCharacter == 0)
					dialogues[i].currentLine++;

				if (dialogues[i].name == "Introduction")
				{
					// Setup text character animation
					if (textTimer >= 30 && textCharacter < dialogues[i].lines[dialogues[i].currentLine].text.length())
					{
						textCharacter++;
						str = dialogues[i].lines[dialogues[i].currentLine].text;
						speechText.setString(str.substring(0, textCharacter));
						clock.restart();
					}
				}
			}
			amountTextChar = str.getSize();
		}
		//Play Dialogue Phase of Introduction 3
		if (gameplay.introductionNumber == 3)
		{
			// Start with first introdution line
			for (int i = 0; i != dialogues.size(); i++)
				// Look for all dialogue lines named Player
				if (dialogues[i].name == "Introduction")
				{
					if (textCharacter == 0 && dialogues[i].currentLine != dialogues[i].lines.size() - 1)
						dialogues[i].currentLine++;

					// Setup text character animation
					if (textTimer >= 30 && textCharacter < dialogues[i].lines[dialogues[i].currentLine].text.length())
					{
						textCharacter++;
						str = dialogues[i].lines[dialogues[i].currentLine].text;
						speechText.setString(str.substring(0, textCharacter));
						clock.restart();
					}
					// Show next line while pressing Enter
					if (textCharacter == dialogues[i].lines[dialogues[i].currentLine].text.length())
					{
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !keyDown
							&& dialogues[i].currentLine != dialogues[i].lines.size() - 1)
							textCharacter = 0;
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !keyDown
							&& dialogues[i].currentLine == dialogues[i].lines.size() - 1) {
							clock.restart();
							textCharacter = 0;
							currentQuest = 1;
							gameplay.introductionNumber = 0;
						}
					}
				}
			amountTextChar = str.getSize();
		}

		//------------------QUEST BEHAVIOUR---------------//
		//for each quest number get the character belonging to the quest
		for (int i = 1; i < 17; i++)
		{
			if (currentQuest == i) {
				characterName = characters[i - 1].name;
				characterNumber = i - 1;
			}
		}

		// Loop through all quests depending on currentQuest number
		if (currentQuest != 0)
		{
			cout << mistakes << endl;
			// Show quest text of the current character
			if (!gameplay.correct && !gameplay.wrong && !gameplay.newItem && !gameplay.inventory)
				for (int i = 0; i != dialogues.size(); i++)
				{
					// Look for the dialogueLine name that has the same Name as the character you're loooking for
					if (dialogues[i].name == characterName)
					{
						// display text with animation
						if (textTimer >= 30 && textCharacter < dialogues[i].lines[dialogues[i].currentLine].text.length() + 1)
						{
							str = dialogues[i].lines[dialogues[i].currentLine].text;
							speechText.setString(str.substring(0, textCharacter));
							textCharacter++;
							clock.restart();
						}
					}

				}
			if (mouseLeftDown && !gameplay.correct && amountTextChar == str.getSize())
			{
				// If right character is found, return correct
				if (mouse.intersects(characters[characterNumber].sprite.getGlobalBounds()) && !gameplay.inventory)
				{
					textCharacter = 0;
					gameplay.wrong = false;
					gameplay.correct = true;
				}
				// If clicked on wrong character, return wrong 
				else if (!mouse.intersects(characters[characterNumber].sprite.getGlobalBounds()) && !gameplay.inventory)
				{
					gameplay.correct = false;
					gameplay.wrong = true;
				}
			}

			// If correct; show textlines for correct
			if (gameplay.correct && !gameplay.inventory)
			{
				for (int i = 0; i != dialogues.size(); i++)
					if (dialogues[i].name == "Right")
					{
						// set str to "you found" + "Character name" + !
						str = dialogues[i].lines[dialogues[i].currentLine].text + " " + characters[characterNumber].name + "! ...";
						if (textTimer >= 30 && textCharacter < str.getSize())
						{
							speechText.setString(str.substring(0, textCharacter));
							textCharacter++;
							clock.restart();
						}
						// If enter is pressed, continue with next quest
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !keyDown
							&& textCharacter >= str.getSize())
						{
							// show new Item text
							gameplay.newItem = true;
							gameplay.correct = false;
						}
					}
			} // chech if character that has been found contained an item
			// if yes add item to inventory
			if (gameplay.newItem && !gameplay.correct && !gameplay.wrong)
			{
				for (int i = 0; i < characters.size(); i++)
				{
					if (characters[i].name == characterName)
						if (characters[i].item == 1)
						{
							str = "You have a new letter in your inventory!\nPress [E] ";
							if (textTimer >= 30 && textCharacter < str.getSize()) {
								speechText.setString(str.substring(0, textCharacter));
								textCharacter++;
								clock.restart();
							}
							if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && gameplay.inventory) {
								// add item
								gameplay.numberItems = gameplay.numberItems + 1;
								// add quest number
								currentQuest = currentQuest + 1;
								textCharacter = 0;
								gameplay.newItem = false;
							}
						}
						else if (characters[i].item == 0) {
							//add quest number
							currentQuest = currentQuest + 1;
							textCharacter = 0;
							gameplay.newItem = false;
						}
				}
			}
			if (gameplay.wrong && !gameplay.inventory && !gameplay.newItem)
			{
				if (mistakes == 4)
					for (int i = 0; i != dialogues.size(); i++)
					{
						// Look for the dialogueLine name that has the same Name as the character you're loooking for
						if (dialogues[i].name == characterName)
						{
							// display text with animation
							if (textTimer >= 30 && textCharacter < dialogues[i].lines[dialogues[i].currentLine].text.length() + 1)
							{
								str = dialogues[i].lines[dialogues[i].currentLine].text;
								speechText.setString(str.substring(0, textCharacter));
								textCharacter++;
								clock.restart();
							}
							if(textCharacter >= str.getSize() && mouseLeftDown &&
								!mouse.intersects(characters[characterNumber].sprite.getGlobalBounds()))
							{
								textCharacter = 0;
								mistakes = 0;
							}
						}
					}
				if (mistakes != 4)
				{
					for (int i = 0; i != dialogues.size(); i++)
						if (dialogues[i].name == "Wrong")
						{
							// set str to "you found" + "Character name" + !
							str = dialogues[i].lines[dialogues[i].currentLine].text;
							if (textTimer >= 30 && textCharacter < str.getSize() + 1)
							{
								speechText.setString(str.substring(0, textCharacter));
								textCharacter++;
								clock.restart();
							}
							if (textCharacter >= str.getSize() && mouseLeftDown &&
								!mouse.intersects(characters[characterNumber].sprite.getGlobalBounds()))
							{
								textCharacter = 0;
								if (dialogues[i].currentLine == dialogues[i].lines.size() - 1)
									dialogues[i].currentLine = -1;

								if (dialogues[i].currentLine != dialogues[i].lines.size() - 1)
								{
									mistakes = mistakes + 1;
									dialogues[i].currentLine++;
								}
							}
						}
				}


			}
			amountTextChar = str.getSize();
		}

		if (currentQuest == 16)
		{

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



