#pragma once

#include "entity.h"
#include "random.h"
#include <iostream>

class targets : public entity
{
public:
public:
	string name;
	string textContent;
	float width;
	float height;
	int counter = 0;
	int counterWalking = 0;
	float movementSpeed = 0.01f;

	int movementLength = 200;
	int direction = 0; // 1 - up, 2 - down, 3 - left, 4 - right
	//int attackDamage = 5;

	sf::Clock clock;
	int timer;

	bool isMoving;

	targets();
	void update();
	void updateMovement();
	void walkCycle();
};

