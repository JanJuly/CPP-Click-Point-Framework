#pragma once
#include "entity.h"
#include <iostream>

class Player : public entity
{
public:
	float moveSpeed = 3;
	int walkingCounter = 0;
	int walkingCounterPersistent = 0;

	sf::SoundBuffer step;
	sf::Sound stepSound;

	Player();
	void update();

};