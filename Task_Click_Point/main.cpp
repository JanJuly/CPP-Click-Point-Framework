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
#include "targets.h"
#include "random.h"
#include "Dialogue.h"
#include "Inventory.h"
#include "Sprite.h"

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

	// characters import
	vector<SpriteContent> characterSprites = ReadXmlSpriteContent("Character");
	vector<targets> characters(characterSprites.size());
	// convert generic spritecontents into target
	for (int s = 0; s != characters.size(); s++) {
		characters[s].name = characterSprites[s].name;
		characters[s].height = characterSprites[s].spriteHeight;
		characters[s].width = characterSprites[s].spriteWidth;
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
	int amountCharacters = 0;

	// call entity class for gameplay state
	class entity gameplay;

	//Setup background images
	sf::Texture background_1;
	sf::Texture background_2;
	sf::Texture background_3;

	// load background textures out of file Sprites
	if (!background_1.loadFromFile("Sprites\\background.png"))
		return EXIT_FAILURE;
	if (!background_2.loadFromFile("Sprites\\background2.png"))
		return EXIT_FAILURE;
	if (!background_3.loadFromFile("Sprites\\background3.png"))
		return EXIT_FAILURE;

	// set texture of background sprite
	sf::Sprite sprite_background(background_1);
	sf::Sprite sprite_background_2(background_2);
	sf::Sprite sprite_background_3(background_3);

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

	//setup background music
	sf::Music music;
	string musicSource = "Sounds\\music_1.wav";
	music.openFromFile(musicSource);
	music.play();
	music.setVolume(20);
	music.setLoop(true);

	//setup sounds
	sf::SoundBuffer buffer;
	buffer.loadFromFile("Sounds\\key.ogg");
	sf::Sound sound;
	sound.setBuffer(buffer);
	sound.setVolume(100);
	sound.setPitch(1.3);
	sound.setLoop(true);



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

		string musicSource = "";
		if (musicSource != "")
		{
			music.openFromFile(musicSource);
			music.play();
		}

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

		//Scene 1 == 1
		for (int i = 1; i < 3; i++)
		{
			//if player walks to the right -> get next scene to the right
			if (player.sprite.getPosition().x >= window.getSize().x - 80)
			{
				// if last scene, start from scene 1
				if (gameplay.sceneNumber == 3)
					gameplay.sceneNumber = 1;
				// else continue to next scene
				else
					gameplay.sceneNumber = gameplay.sceneNumber + 1;
				// stop music in order to change
				music.pause();
				// define music source
				music.openFromFile("Sounds\\music_" + to_string(gameplay.sceneNumber) + ".wav");
				// set sprite position to the left of the screen
				player.sprite.setPosition(1, player.sprite.getPosition().y);
			}
			//if player walks to the left -> get next scene to the left
			if (player.sprite.getPosition().x <= window.getSize().x - window.getSize().x)
			{
				// change scene backwards 
				if (gameplay.sceneNumber == 1)
					gameplay.sceneNumber = 3;
				else
					gameplay.sceneNumber = gameplay.sceneNumber - 1;

				music.pause();
				music.openFromFile("Sounds\\music_" + to_string(gameplay.sceneNumber) + ".wav");
				// set sprite position to the right of the screen
				player.sprite.setPosition(window.getSize().x - 81, player.sprite.getPosition().y);
			}
		}

		// Enable movement of player as soon as the first introduction lines have been shown
		if (gameplay.introductionNumber != 1)
			player.update();


		//-------------------DRAW SCENE 1-------------------//
		if (gameplay.sceneNumber == 1)
		{
			// change background music
			if (music.getStatus() == sf::Sound::Status::Stopped)
				music.play();

			//Draw Background
			window.draw(sprite_background);
			//Draw all characters of scene 1
			for (int i = 0; i < 7; i++)
				window.draw(characters[i].sprite);
		}


		//-------------------DRAW SCENE 2-------------------//
		if (gameplay.sceneNumber == 2)
		{
			// change background music
			if (music.getStatus() == sf::Sound::Status::Stopped)
				music.play();

			// Draw background
			window.draw(sprite_background_2);

			// draw characters of scene 2
			window.draw(characters[7].sprite);

			// show next introduction lines
			if (gameplay.introductionNumber == 1)
			{
				gameplay.introductionNumber = 2;
				clock.restart();
				textCharacter = 0;
			}
		}

		//-------------------DRAW SCENE 3-------------------//
		if (gameplay.sceneNumber == 3)
		{
			// change background music
			if (music.getStatus() == sf::Sound::Status::Stopped)
				music.play();

			// Draw background
			window.draw(sprite_background_3);

			// Draw characters of scene 3
			for (int i = 8; i < characters.size(); i++)
				window.draw(characters[i].sprite);

			if (gameplay.introductionNumber == 2)
			{
				gameplay.introductionNumber = 3;
				clock.restart();
				textCharacter = 0;
			}
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
		if (textCharacter <  amountCharacters && sound.getStatus() != sf::Sound::Status::Playing)
			sound.play();
		if (textCharacter >= amountCharacters)
			sound.stop();


		//---------------INVENTORY BEHAVIOUR----------------//

		// enable counter to open/close inventory, prevents of closing and opening too fast
		gameplay.openInventory++;
		if (gameplay.openInventory >= 20 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && currentQuest >= 1)
		{
			if (!gameplay.inventory)
				gameplay.inventory = true; //open Inventory
			else
				gameplay.inventory = false; //close Inventory

			// if opened/closed reset counter
			gameplay.openInventory = 0;
		}

		// If inventory if open
		if (gameplay.inventory)
		{
			// Draw inventory template background
			window.draw(inventorySprite);

			// Draw Items according to amount of quest numbers fullfilled
			for (int i = 0; i < items.size(); i++) {
				if (currentQuest >= i + 2) {
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
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !keyDown &&
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
			amountCharacters = str.getSize();
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
			amountCharacters = str.getSize();
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
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !keyDown
							&& dialogues[i].currentLine != dialogues[i].lines.size() - 1)
							textCharacter = 0;
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !keyDown
							&& dialogues[i].currentLine == dialogues[i].lines.size() - 1) {
							clock.restart();
							textCharacter = 0;
							currentQuest = 1;
							gameplay.introductionNumber = 0;
						}
					}
				}
			amountCharacters = str.getSize();
		}

		//------------------QUEST BEHAVIOUR---------------//

		if (currentQuest == 1) { // If quest number is 1
			characterName = characters[0].name; // Set the characterName to the same name as in DialogueLine
			characterNumber = 0; // Get number of current character
		}
		if (currentQuest == 2) {
			characterName = characters[5].name;
			characterNumber = 5;
		}
		if (currentQuest == 3) {
			characterName = characters[6].name;
			characterNumber = 6;
		}
		if (currentQuest == 4) {
			characterName = characters[11].name;
			characterNumber = 11;
		}
		if (currentQuest == 5) {
			characterName = characters[9].name;
			characterNumber = 9;
		}
		if (currentQuest == 6) {
			characterName = characters[10].name;
			characterNumber = 10;
		}
		if (currentQuest == 7) {
			characterName = characters[7].name;
			characterNumber = 7;
		}
		if (currentQuest == 8) {
			characterName = characters[14].name;
			characterNumber = 14;
		}
		if (currentQuest == 9) {
			characterName = characters[13].name;
			characterNumber = 13;
		}
		if (currentQuest == 10) {
			characterName = characters[12].name;
			characterNumber = 12;
		}

		// Loop through all quests depending on currentQuest number
		if (currentQuest != 0)
		{
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
			if (mouseLeftDown && !gameplay.correct && !gameplay.inventory)
			{
				// If right character is found, return correct
				if (mouse.intersects(characters[characterNumber].sprite.getGlobalBounds()))
				{
					textCharacter = 0;
					gameplay.wrong = false;
					gameplay.correct = true;
				}
				// If clicked on wrong character, return wrong 
				if (!mouse.intersects(characters[characterNumber].sprite.getGlobalBounds()))
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
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !keyDown
							&& textCharacter >= str.getSize())
						{
							//add quest number
							currentQuest = currentQuest + 1;
							// show new Item text
							gameplay.newItem = true;
							gameplay.correct = false;
						}
					}
			}
			if (gameplay.newItem)
			{
				str = "You have a new letter in your inventory!\nPress [SPACE]";
				if (textTimer >= 30 && textCharacter < str.getSize())
				{
					speechText.setString(str.substring(0, textCharacter));
					textCharacter++;
					clock.restart();
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && gameplay.inventory)
				{
					textCharacter = 0;
					gameplay.newItem = false;
				}

			}
			if (gameplay.wrong && !gameplay.inventory)
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
								dialogues[i].currentLine++;
						}
					}

			}
			amountCharacters = str.getSize();
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



