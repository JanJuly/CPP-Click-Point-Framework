#pragma once

#include "entity.h"
#include "random.h"
#include <iostream>

class targets : public entity
{
public:
	// values inherited from xml
	string name;
	string textContent;
	float width;
	float height;
	int sceneNumber;

	int counter = 0;
	int walkingCounter = 0;
	int walkingCounterPersistent = 0;
	float moveSpeed = 0.8;

	sf::Texture texture;
	string textureName;

	int movementLength = 200;
	int direction = 0; // 1 - up, 2 - down, 3 - left, 4 - right
	//int attackDamage = 5;


	bool isMoving;

	targets();
	void update();
	void updateMovement();
	void walkCycle();
};

