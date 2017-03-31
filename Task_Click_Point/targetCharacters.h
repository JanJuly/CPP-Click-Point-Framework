#pragma once
#include "entity.h"
#include <iostream>

class targetCharacters : public entity
{
public:
	float moveSpeed = 0.1;
	int walkingCounter = 0;
	int walkingCounterPersistent = 0;
	int refreshSpeed = 3;
	bool isMoving = false;
	targetCharacters();
	//void update();
	//void updateMove();

};