#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <iostream>

#include "entity.h"
#include "Player.h"

using namespace std;
int main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 600), "Click & Point Programming Task");

	//BACKGROUND SPRITES
	sf::Texture texture_background;
	if (!texture_background.loadFromFile("background.jpg"))
		return EXIT_FAILURE;
	sf::Sprite sprite_background(texture_background);

	//FONT & TEXT
	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
		return EXIT_FAILURE;
	sf::Text text("Hello", font, 50);
	text.setFillColor(sf::Color::Red);
	text.setPosition(300, 300);


	//CHARACTER SPRITE
	sf::Texture texturePlayer;
	if (!texturePlayer.loadFromFile("spriteWalk.png"))
		return EXIT_FAILURE;


	class Player character;
	character.sprite.setTexture(texturePlayer);
	character.sprite.setPosition(window.getSize().x / 2, window.getSize().y / 2 + 200);
	character.sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();


		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !character.isMoving)
		{
			character.mousePosition = sf::Mouse::getPosition(window);
			cout << character.mousePosition.x << " " << character.mousePosition.y << endl;
		}







		character.update();
		character.updateMove();

		//Draw Background
		window.draw(sprite_background);

		//Draw Text
		window.draw(text);
		window.draw(character.sprite);


		window.display();
	}


	return 0;
}