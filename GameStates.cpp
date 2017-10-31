#include "GameStates.h"
#include <iostream>
#include <sstream>

using namespace sf;

GameStates::GameStates(void)
{
	font.loadFromFile("Assets/BebasNeueBook.ttf");
	buffer.loadFromFile("Assets/Klik.wav");
	state = END;
	click.setBuffer(buffer);
	window.create(sf::VideoMode{ 800,600 }, "Pong");
	window.setFramerateLimit(60);
	state = MENU;
}


GameStates::~GameStates()
{

}

void GameStates::runGame()
{
	while (state != END)
	{
		switch (state)
		{
		case Gamestates::MENU:
			menu();
			break;
		case Gamestates::GAME:
			game();
			break;
		case Gamestates::INFO:
			info();
			break;

		}
	}
}

std::string GameStates::stringPorting(int number)
{
	std::stringstream ss;
	ss << number;

	return ss.str();

}

void GameStates::game()
{
	bool Right = false;
	bool Left = false;


	sf::Text cPoints;
	sf::Text pPoints;
	cPoints.setFont(font);
	pPoints.setFont(font);
	cPoints.setPosition(0, 0);
	pPoints.setPosition(0, 40);
	cPoints.setCharacterSize(45);
	pPoints.setCharacterSize(45);


	RectangleShape Player(Vector2f(120, 20));
	Player.setPosition(350, 540);

	RectangleShape Enemy(Vector2f(120, 20));
	Enemy.setPosition(350, 40);

	int ballspeed_y = -10;
	int ballspeed_x = 10;

	bool ballIsMoving = false;

	CircleShape Ball;
	Ball.setRadius(10);

	while (state == GAME)
	{
		window.setKeyRepeatEnabled(false);

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				state = END;
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					click.play();
					state = MENU;
				}
				else if (event.key.code == Keyboard::D)
				{
					Right = true;
					break;
				}
				else if (event.key.code == Keyboard::A)
				{
					Left = true;
					break;
				}
				else if (event.key.code == Keyboard::Space)
					ballIsMoving = true;
				else if (event.key.code == Keyboard::R)
				{
					ballIsMoving = false;
				}
			}
			if (event.type == Event::KeyReleased)
			{
				if (event.key.code == Keyboard::A)
					Left = false;

				else if (event.key.code == Keyboard::D)
					Right = false;

			}


		}

		if (Left == true)
		{
			if (Player.getPosition().x > 0)
				Player.move(-10, 0);
		}
		else if (Right == true)
		{
			if (Player.getPosition().x < 800 - Player.getSize().x)
				Player.move(10, 0);
		}

		if (!ballIsMoving)
			Ball.setPosition(Player.getPosition().x + Player.getSize().x / 2 - 10, Player.getPosition().y - 40);
		else if (ballIsMoving)
			Ball.move(ballspeed_x, ballspeed_y);


		if (Ball.getPosition().x <= 0)
			ballspeed_x = -ballspeed_x;
		else if (Ball.getPosition().x >= 790)
			ballspeed_x = -ballspeed_x;
		else if (Ball.getGlobalBounds().intersects(Player.getGlobalBounds()))
			ballspeed_y = -10;
		else if (Ball.getGlobalBounds().intersects(Enemy.getGlobalBounds()))
			ballspeed_y = 10;
		else if (Ball.getPosition().y > 650)
		{
			computerPoints++;
			ballIsMoving = false;
			ballspeed_y = -10;
		}
		else if (Ball.getPosition().y < -20)
		{
			playerPoints++;
			ballIsMoving = false;
			ballspeed_y = -10;
		}
		if (ballIsMoving)
		{
			if (Enemy.getPosition().x > Ball.getPosition().x - 40)
				Enemy.move(-10, 0);
			else if (Enemy.getPosition().x < Ball.getPosition().x - 40)
				Enemy.move(10, 0);
		}
		cPoints.setString("Computer: " + stringPorting(computerPoints));
		pPoints.setString("You: " + stringPorting(playerPoints));


		window.clear();
		window.draw(cPoints);
		window.draw(pPoints);
		window.draw(Player);
		window.draw(Enemy);
		window.draw(Ball);
		window.display();
	}
	computerPoints = 0;
	playerPoints = 0;
}

void GameStates::menu()
{
	const int howMuch = 3;

	sf::Text text[howMuch];

	std::string str[] = { "Play","Info","Exit" };
	sf::Text logo;
	std::string pong = "Pong";
	logo.setString(pong);
	logo.setFont(font);
	logo.setCharacterSize(100);
	logo.setPosition(335,30);
	for (int i = 0; i<howMuch; i++)
	{
		text[i].setFont(font);
		text[i].setCharacterSize(50);

		text[i].setString(str[i]);
		text[i].setPosition(800 / 2 - text[i].getGlobalBounds().width / 2, 150 + i * 100);
	}

	while (state == MENU)
	{
		window.clear();
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				state = END;
			else if (text[0].getGlobalBounds().contains(mouse) &&
				event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				click.play();
				state = GAME;
			}

			else if (text[1].getGlobalBounds().contains(mouse) &&
				event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				click.play();
				state = INFO;
			}
			else if (text[2].getGlobalBounds().contains(mouse) &&
				event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				state = END;
			}
		}
		for (int i = 0; i<howMuch; i++)
			if (text[i].getGlobalBounds().contains(mouse))
				text[i].setFillColor(sf::Color::Yellow);
			else text[i].setFillColor(sf::Color::White);
			for (int i = 0; i < howMuch; i++)
			{
				window.draw(text[i]);
			}
			window.draw(logo);
			window.display();
	}
}

void GameStates::info()
{
	sf::Sprite info;
	sf::Texture infoTexture;
	sf::Sprite backToMenu;
	sf::Texture backToMenuTexture;
	backToMenuTexture.loadFromFile("Assets/menu.png");
	infoTexture.loadFromFile("Assets/info.png");
	info.setTexture(infoTexture);
	backToMenu.setTexture(backToMenuTexture);
	backToMenu.setScale(0.5f, 0.5f);
	while (state == INFO)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		window.clear();
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				state = END;
			if (backToMenu.getGlobalBounds().contains(mouse) &&
				event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				click.play();
				state = MENU;
			}
		}
		if (backToMenu.getGlobalBounds().contains(mouse))
			backToMenu.setColor(sf::Color::Yellow);
		else backToMenu.setColor(sf::Color::White);
		window.draw(info);
		window.draw(backToMenu);
		window.display();
	}
}
