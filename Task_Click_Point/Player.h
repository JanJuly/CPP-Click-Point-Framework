#pragma once
#include "entity.h"
#include <iostream>

class Player : public entity
{
public:
	float moveSpeed = 0.01f;
	int walkingCounter = 0;
	int walkingCounterPersistent;
	int refreshSpeed = 20;
	bool isMoving = false;
	Player();
	void update();
	void updateMove();

};