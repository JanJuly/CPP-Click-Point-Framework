#include "Player.h"

Player::Player()
{
	sprite.setTextureRect(sf::IntRect(32, 0, 32, 32));

	step.loadFromFile("Sounds\\step.wav");
	stepSound.setBuffer(step);
	stepSound.setVolume(100);
	stepSound.setPitch(2);
	stepSound.setLoop(true);
}


void Player::update()
{

	// walk player left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		if (stepSound.getStatus() != sf::Sound::Status::Playing)
			stepSound.play();

		walkingCounterPersistent++;
		sprite.move(-moveSpeed, 0);
		sprite.setTextureRect(sf::IntRect(walkingCounter * 32, 32 * 1, 32, 32));
	}
	// walk player right
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (stepSound.getStatus() != sf::Sound::Status::Playing)
			stepSound.play();
		walkingCounterPersistent++;
		sprite.move(moveSpeed, 0);
		sprite.setTextureRect(sf::IntRect(walkingCounter * 32, 32 * 2, 32, 32));
	}
	// get front sprite if not walking
	else
	{
		sprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
		stepSound.stop();
	}


	if (walkingCounterPersistent == 8) {
		walkingCounterPersistent = 0;
		walkingCounter++;
	}
	if (walkingCounter == 2)
		walkingCounter = 0;



}


//WALK CHARACTER WITH MOUSE CLICK ON X-AXIS
/*if (isMoving)
{
	//COUNTER FOR ANIMATION

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
	}*/

	/*	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMoving)
			isMoving = true;*/


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
			}*/





