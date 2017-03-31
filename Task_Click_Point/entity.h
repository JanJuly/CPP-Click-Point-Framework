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

	// Set current scene to true
	bool scene_1 = true;
	bool scene_2 = false;
	bool scene_3 = false;

	// Set current introduction lines to true
	bool introduction_1 = true;
	bool introduction_2 = false;
	bool introduction_3 = false;

	// set bool to true if guess is correct or false
	bool correct = false;
	bool wrong = false;
	bool newItem = false;

	// if inventory is open, set to true
	bool inventory = false;
	int openInventory = 0; // count to open/close inventory
	int openFile = 0;

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

