#pragma once
#include "entity.h"
#include <iostream>

class Player : public entity
{
public:
	float moveSpeed = 0.01;
	int walkingCounter = 0;
	int walkingCounterPersistent = 0;
	int refreshSpeed = 20;
	bool isMoving = false;
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	Player();
	void update();
	void updateMove();

};