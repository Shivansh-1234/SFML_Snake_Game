#pragma once

#include <SFML/Graphics.hpp>
#include "GLOBALS.h"
#include <vector>
#include <deque>
#include <sstream>

#include "Snake.h"

class Application 
{
public:
	Application();
	~Application();

	sf::RenderWindow window;
	sf::Event event;

	void Run();

	static Application* instance;

private:
	//GameLoop
	sf::Time TimeSinceLastMove;
	bool m_running;
	bool m_paused;
	bool m_gameOver;

	//Snake
	int m_snakeDirection;
	float m_snakeSpeed;
	std::deque<int> m_directionQueue;
	enum Direction { UP, DOWN, LEFT, RIGHT };
	enum Speed {INCREASE, DECREASE};

	std::vector<Snake> m_snakeVec;

	//Apple
	sf::RectangleShape m_apple;

	//score
	size_t   m_score;
	sf::Text m_scoreText;
	sf::Font m_scoreFont;

	//GameOver
	sf::Text m_GameOverText;

	//member functions
	void createNewSnake(Vec2 position);
	void addNewSnakeSection();
	void addDirection(int newDirection);
	void moveSnake();
	void checkSnakeBounds();
	void changeSnakeSpeed(int choice);

	void spawnApple();
	void checkSnakeAppleCollision();

	void initWindow();
	void initVariables();

	void quitGame();

	void HandleEvents();
	void Update();
	void Render();

};