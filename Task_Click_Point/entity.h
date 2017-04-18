#pragma once

#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <deque>
#include <list>
#include <vector>

using namespace std;


class entity
{
public:
	sf::RenderWindow window;
	sf::RectangleShape rect;
	sf::Sprite sprite;
	sf::Text text;
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	sf::Event event;


	int textCounter = 0;
	int timer;
	sf::Clock clock;


	//-------GAMEPLAY--------//

	// Set current scene number (starting with scene 1)
	int sceneNumber = 0;
	//introduction phase
	bool introduction = true;
	// Set Itemnumber that are active
	int numberItems = -1;


	// set bool to true if guess is correct or false
	bool correct = false;
	// if inventory is open, set to true
	bool inventory = false;
	// count to open/close inventory -> I'm starting with 1 because the sound will only be played if counter is 0
	int openInventory = 1; 
	int openFile = 0;
	int firstLine = 0; //start quest line if 0

};

//-------------SETUP MOUSE HITBOX----------------//
class MyMouse {

public:
	MyMouse(const sf::Window& _window, int mouse_width, int mouse_height) :
		window(_window)
	{
		// initialize box around mouse
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

