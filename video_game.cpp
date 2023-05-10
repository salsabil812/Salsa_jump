#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <random>
#include <ctime>
#include<iostream>
#include <sstream>



int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 700), "Salsa Jump", sf::Style::Close);
	window.setFramerateLimit(60);
	sf::Texture backgroundTexture;
	sf::Texture playerTexture;
	sf::Texture platformTexture;
	backgroundTexture.loadFromFile("images/background3.png");
	playerTexture.loadFromFile("images/doodle.png");
	platformTexture.loadFromFile("images/platform.png");
	sf::Sprite background(backgroundTexture);
	sf::Sprite player(playerTexture);
	sf::Sprite platform(platformTexture);

	sf::RectangleShape gameoverBackground(sf::Vector2f(500, 700));
	gameoverBackground.setFillColor(sf::Color::White);

	sf::Font font;
	font.loadFromFile("font/arial.ttf");
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(50);
	scoreText.setFillColor(sf::Color::Black);
	sf::Texture gameoverTexture;
	gameoverTexture.loadFromFile("images/gameover.png");
	sf::Sprite gameoverSprite(gameoverTexture);
	gameoverSprite.setPosition(50.0f, 150.0f);


	sf::SoundBuffer buffer;
	buffer.loadFromFile("audio/jump.wav");
	sf::Sound sound;
	sound.setBuffer(buffer);


	sf::Vector2u platformPosition[10];
	std::uniform_int_distribution<unsigned> x(0, 500 - platformTexture.getSize().x);
	std::uniform_int_distribution<unsigned> y(100, 700);
	std::default_random_engine e(time(0));
	for (size_t i = 0; i < 10; ++i)
	{
		platformPosition[i].x = x(e);
		platformPosition[i].y = y(e);
	}


	int playerX = 250;
	int playerY = 151;
	float dy = 0;
	int height = 150;
	int score = 0;


	const int PLAYER_LEFT_BOUNDING_BOX = 20;
	const int PLAYER_RIGHT_BOUNDING_BOX = 60;
	const int PLAYER_BOTTOM_BOUNDING_BOX = 70;



	
	std::vector<sf::Texture> obstacleTextures(5);
	for (int i = 0; i < 5; i++)
	{
		obstacleTextures[i].loadFromFile("images/louled" + std::to_string(i) + ".png");

	}
	std::vector<sf::Sprite> obstacle;

	sf::Texture resetTexture;
	resetTexture.loadFromFile("images/reset.png");
	sf::Sprite resetSprite(resetTexture);
	resetSprite.setPosition(170.0f,40.0f);
	
	
game:
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();


		}
		

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			playerX -= 4;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			playerX += 4;


		if (playerX > 500)
			playerX = 0;
		if (playerX < -40)
			playerX = window.getSize().x - playerTexture.getSize().x;


		if (playerY == height && dy < (-1.62))
		{
			score += 1;
			scoreText.setString("Score: " + std::to_string(score));
		}


		dy += 0.2;
		playerY += dy;

		if (playerY < height)
			for (size_t i = 0; i < 10; ++i)
			{
				playerY = height;
				platformPosition[i].y -= dy;
				if (platformPosition[i].y > 700)
				{
					platformPosition[i].y = 0;
					platformPosition[i].x = x(e);
				}

			}

		for (size_t i = 0; i < 10; ++i)
		{
			if ((playerX + PLAYER_RIGHT_BOUNDING_BOX > platformPosition[i].x) && (playerX + PLAYER_LEFT_BOUNDING_BOX < platformPosition[i].x + platformTexture.getSize().x)        // player's horizontal range can touch the platform
				&& (playerY + PLAYER_BOTTOM_BOUNDING_BOX > platformPosition[i].y) && (playerY + PLAYER_BOTTOM_BOUNDING_BOX < platformPosition[i].y + platformTexture.getSize().y)  // player's vertical   range can touch the platform
				&& (dy > 0))
			{
				sound.play();
				dy = -10;
			}

		}
		player.setPosition(playerX, playerY);

		window.draw(background);
		window.draw(player);


		for (size_t i = 0; i < 10; ++i)
		{
			platform.setPosition(platformPosition[i].x, platformPosition[i].y);
			window.draw(platform);
		}
		static sf::Clock clock;

		if (clock.getElapsedTime().asSeconds() >= 2)
		{
			
			int randomIndex = std::rand() % obstacleTextures.size();
			sf::Texture& obstacleTexture = obstacleTextures[randomIndex];
			sf::Sprite obstacles(obstacleTexture);
			obstacles.setScale(0.5f, 0.5f);
			obstacles.setPosition(rand() % 600, 0);
			obstacle.push_back(obstacles);
			clock.restart();
		}
		for (auto& obstacles : obstacle)
		{
			obstacles.move(0, 2); 
			window.draw(obstacles);

			if (player.getGlobalBounds().intersects(obstacles.getGlobalBounds())) {
				gameoverSprite.setPosition(0, 200);
				scoreText.setPosition(135, 130);
				goto gameover;
			}
		}


		if (playerY > 700)
		{
			gameoverSprite.setPosition(0, 200);
			scoreText.setPosition(135, 130);
			goto gameover;
		}
		
		






		window.draw(scoreText);
		window.display();

	}






gameover:
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if ((event.type == sf::Event::Closed))
				window.close();


			
			
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
					if (resetSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
					{
						
						playerX = 250;
						playerY = 151;
						dy = 0;
						score = 0;
						scoreText.setString("Score: " + std::to_string(score)); 
						scoreText.setPosition(0,0); 
						for (size_t i = 0; i < 10; ++i)
						{
							platformPosition[i].x = x(e);
							platformPosition[i].y = y(e);
						}
						
						obstacle.clear();
						
						for (int i = 0; i < 5; i++)
						{
							sf::Sprite ob(obstacleTextures[i]);
							ob.setPosition(x(e), -300 - i * 400);
							ob.setScale(0.5f, 0.5f);
							obstacle.push_back(ob);
						}
						
						goto game;
					}
				}


				window.draw(gameoverBackground);
				window.draw(gameoverSprite);
				window.draw(scoreText);
				window.draw(resetSprite);
				window.display();
			}
		}
	

	return 0;
}
