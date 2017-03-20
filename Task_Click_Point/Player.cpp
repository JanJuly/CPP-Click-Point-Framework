#include "Player.h"

Player::Player()
{
	rect.setSize(sf::Vector2f(32, 32));
}

void Player::update()
{
	
}

void Player::updateMove()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMoving)
		isMoving = true;


	//WALK CHARACTER WITH MOUSE CLICK ON X-AXIS
	if (isMoving)
	{
		//COUNTER FOR ANIMATION
		walkingCounterPersistent++;
		if (walkingCounterPersistent == refreshSpeed) {
			walkingCounterPersistent = 0;
			walkingCounter++;
		}
		if (walkingCounter == 3)
			walkingCounter = 0;

		//WALK CHARACTER LEFT
		if (sprite.getPosition().x > mousePosition.x)
		{
			sprite.move(-moveSpeed, 0); //Move in -X direction
			sprite.setTextureRect(sf::IntRect(walkingCounter * 32, 32 * 1, 32, 32)); //Loop through sprite sheet for animation
			if ((sprite.getPosition().x - mousePosition.x) < 0.1f) //If distance to mouseclick position and sprite is smaller than 0.1f, stop moving
			{
				isMoving = false;
				walkingCounter = 0;
				walkingCounterPersistent = 0;
				refreshSpeed = 20;
			}

		}
		//WALK CHARACTER RIGHT
		if (sprite.getPosition().x < mousePosition.x)
		{
			sprite.move(moveSpeed, 0); //Move in +X direction
			sprite.setTextureRect(sf::IntRect(walkingCounter * 32, 32 * 2, 32, 32));
			if ((mousePosition.x - sprite.getPosition().x) < 0.1f)
			{
				isMoving = false;
				walkingCounter = 0;
				walkingCounterPersistent = 0;
				refreshSpeed = 20;
			}
		}


	}

	//WALK CHARACTER WITH KEYBOARD
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		sprite.move(0, -moveSpeed);
		sprite.setTextureRect(sf::IntRect(walkingCounter * 32, 32 * 3, 32, 32));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		sprite.move(0, moveSpeed);
		sprite.setTextureRect(sf::IntRect(walkingCounter * 32, 0, 32, 32));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		sprite.move(-moveSpeed, 0);
		sprite.setTextureRect(sf::IntRect(walkingCounter * 32, 32 * 1, 32, 32));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		sprite.move(moveSpeed, 0);
		sprite.setTextureRect(sf::IntRect(walkingCounter * 32, 32 * 2, 32, 32));
	}*/




}