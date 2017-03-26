#include "targets.h"


targets::targets()
{
	rect.setSize(sf::Vector2f(width, height));
	sprite.setTextureRect(sf::IntRect(0, 0, width, height));

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
		if (rect.getPosition().y > 50)
		{
			rect.move(0, -movementSpeed);
			sprite.setTextureRect(sf::IntRect(counterWalking * width, height * 3, width, height));
		}
		else
			sprite.setTextureRect(sf::IntRect(0, 0, width, height));
	}
	else if (direction == 2) // Down
	{
		if (rect.getPosition().y < window.getSize().y - 50)
		{
			rect.move(0, movementSpeed);
			sprite.setTextureRect(sf::IntRect(counterWalking * width, 0, width, height));
		}
		else
			sprite.setTextureRect(sf::IntRect(0, 0, width, height));
	}
	else if (direction == 3) // Left
	{
		if (rect.getPosition().x > 50)
		{
			rect.move(-movementSpeed, 0);
			sprite.setTextureRect(sf::IntRect(counterWalking * width, height, width, height));
		}
		else
			sprite.setTextureRect(sf::IntRect(0, 0, width, height));

	}
	else if (direction == 4) // Right
	{
		if (rect.getPosition().x < window.getSize().x - 50)
		{
			rect.move(movementSpeed, 0);
			sprite.setTextureRect(sf::IntRect(counterWalking * width, height * 2, width, height));
		}
		else
			sprite.setTextureRect(sf::IntRect(0, 0, width, height));
	}
	else
		sprite.setTextureRect(sf::IntRect(0, 0, width, height));


	counterWalking++;
	if (counterWalking == 4)
		counterWalking = 0;


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
