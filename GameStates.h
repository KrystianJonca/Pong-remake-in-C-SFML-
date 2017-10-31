#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
class GameStates
{
public:
	GameStates(void);
	~GameStates();
	sf::RenderWindow window;
	sf::Event event;
	sf::Font font;
	int playerPoints, computerPoints;
	sf::SoundBuffer buffer;
	sf::Sound click;
	void runGame();
	enum Gamestates { GAME, MENU, INFO, END };
	Gamestates state;
	std::string stringPorting(int number);
private:
	void game();
	void menu();
	void info();
};
