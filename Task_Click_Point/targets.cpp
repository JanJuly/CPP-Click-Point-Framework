#include "targets.h"


targets::targets()
{
	rect.setSize(sf::Vector2f(width, height));
	sprite.setTextureRect(sf::IntRect(0, 0, width, height));
	sprite.setScale(1.5, 1.5);
}

void targets::update()
{
	sprite.setPosition(rect.getPosition());

	if (direction == 0)
	{
		direction = generateRandom(10);
		counter = generateRandom(5);
	}

	if (!isMoving)
		sprite.setTextureRect(sf::IntRect(0, 0, width, height));

}

void targets::updateMovement()
{
	if (direction == 1) // Up
	{
		if (rect.getPosition().y > 265)
		{
			rect.move(0, -moveSpeed);
			sprite.setTextureRect(sf::IntRect(walkingCounter * width, height * 3, width, height));
			walkingCounterPersistent++;
		}
		else
			sprite.setTextureRect(sf::IntRect(0, 0, width, height));	
	}
	else if (direction == 2) // Down
	{
		if (rect.getPosition().y <= 300)
		{
			rect.move(0, moveSpeed);
			sprite.setTextureRect(sf::IntRect(walkingCounter * width, 0, width, height));
			walkingCounterPersistent++;
		}
		else		
			sprite.setTextureRect(sf::IntRect(0, 0, width, height));
		
	}
	else if (direction == 3) // Left
	{
		if (rect.getPosition().x > 50)
		{
			rect.move(-moveSpeed, 0);
			sprite.setTextureRect(sf::IntRect(walkingCounter * width, height, width, height));
			walkingCounterPersistent++;
		}
		else
			sprite.setTextureRect(sf::IntRect(0, 0, width, height));
	}
	else if (direction == 4) // Right
	{
		if (rect.getPosition().x < window.getSize().x - 50)
		{
			rect.move(moveSpeed, 0);
			sprite.setTextureRect(sf::IntRect(walkingCounter * width, height * 2, width, height));
			walkingCounterPersistent++;
		}
		else
			sprite.setTextureRect(sf::IntRect(0, 0, width, height));
	}
	else
		sprite.setTextureRect(sf::IntRect(0, 0, width, height));



	if (walkingCounterPersistent == 8) {
		walkingCounterPersistent = 0;
		walkingCounter++;
	}
	if (walkingCounter == 4)
		walkingCounter = 0;


}

void targets::walkCycle()
{
	update();
	timer = clock.getElapsedTime().asSeconds();

	if (timer == counter)
	{
		updateMovement();
		isMoving = true;
	}

	if (timer == counter + 2)
	{
		isMoving = false;
		counter = 0;
		direction = 0;
		clock.restart();
	}
}
