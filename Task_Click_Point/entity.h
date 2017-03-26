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

};
