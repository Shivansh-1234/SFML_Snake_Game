#include "../headers/Application.h"

#include <sstream>

Application* Application::instance = nullptr;

Application::Application()
{
	initWindow();
	initVariables();

	createNewSnake(Vec2(100.f, 100.f));
	spawnApple();
}

Application::~Application()
{
}

void Application::Run()
{
	sf::Clock m_dtClock;

	while (window.isOpen() && m_running)
	{
		sf::Time DT = m_dtClock.restart();

		TimeSinceLastMove += DT;

		HandleEvents();
		
		if (!m_paused) {

			if (!m_gameOver) {
				Update();
			}

			window.clear(sf::Color::Black);

			Render();

			window.display();
		}
		
	}
}

void Application::createNewSnake(Vec2 position)
{
	m_snakeVec.clear();
	m_snakeVec.emplace_back(position);
	m_snakeVec.emplace_back(Vec2(position.x - 20.f, position.y));
	m_snakeVec.emplace_back(Vec2(position.x - 40.f, position.y));
}

void Application::addNewSnakeSection()
{
	//Adds a snake section to the back of the snake
	Vec2 newSectionPos = { m_snakeVec[m_snakeVec.size() - 1].getSnakePos().x - 20.f, m_snakeVec[m_snakeVec.size() - 1].getSnakePos().y };
	m_snakeVec.emplace_back(newSectionPos);
}

void Application::addDirection(int newDirection)
{
	if (m_directionQueue.empty()) {
		m_directionQueue.emplace_back(newDirection);
	}

	else {
		if (m_directionQueue.back() != newDirection) {
			m_directionQueue.emplace_back(newDirection);
		}
	}
}

void Application::moveSnake()
{
	if (TimeSinceLastMove.asSeconds() >= sf::seconds(1.f / (float)(m_snakeSpeed)).asSeconds())
	{
		Vec2 thisSectionPos = m_snakeVec[0].getSnakePos();
		Vec2 lastSectionPos = thisSectionPos;

		if (!m_directionQueue.empty())
		{
			switch (m_snakeDirection)
			{
			case Direction::UP:
				if (m_directionQueue.front() != Direction::DOWN) {
					m_snakeDirection = m_directionQueue.front();
				}
				break;

			case Direction::DOWN:
				if (m_directionQueue.front() != Direction::UP) {
					m_snakeDirection = m_directionQueue.front();
				}
				break;

			case Direction::LEFT:
				if (m_directionQueue.front() != Direction::RIGHT) {
					m_snakeDirection = m_directionQueue.front();
				}
				break;

			case Direction::RIGHT:
				if (m_directionQueue.front() != Direction::LEFT) {
					m_snakeDirection = m_directionQueue.front();
				}
				break;
			}

			m_directionQueue.pop_front();
		}

		//Update Snake HeadPos

		switch (m_snakeDirection)
		{
		case Direction::UP:
			m_snakeVec[0].setSnakePos(Vec2(thisSectionPos.x, thisSectionPos.y - 20.f));
			break;
		case Direction::DOWN:
			m_snakeVec[0].setSnakePos(Vec2(thisSectionPos.x, thisSectionPos.y + 20.f));
			break;
		case Direction::LEFT:
			m_snakeVec[0].setSnakePos(Vec2(thisSectionPos.x - 20.f, thisSectionPos.y));
			break;
		case Direction::RIGHT:
			m_snakeVec[0].setSnakePos(Vec2(thisSectionPos.x + 20.f, thisSectionPos.y));
			break;

		}

		//Update Rest of Snake position

		for (int i = 1; i < m_snakeVec.size(); i++)
		{
			thisSectionPos = m_snakeVec[i].getSnakePos();
			m_snakeVec[i].setSnakePos(lastSectionPos);
			lastSectionPos = thisSectionPos;

		}

		TimeSinceLastMove = sf::Time::Zero;
	}
}

void Application::checkSnakeBounds()
{
	Vec2 snakeHead = m_snakeVec[0].getSnakePos();

	//Snake and wall collision
	if (snakeHead.x < 0) {
		m_gameOver = true;
	}

	if (snakeHead.x > Globals::WINDOW_WIDTH) {
		m_gameOver = true;
	}

	if (snakeHead.y > Globals::WINDOW_HEIGHT) {
		m_gameOver = true;
	}

	if (snakeHead.y < 0) {
		m_gameOver = true;
	}

	//Snake with itself collision
	for (int i = 1; i < m_snakeVec.size(); i++) {
		if (m_snakeVec[i].getSection().getGlobalBounds().intersects(m_snakeVec[0].getSection().getGlobalBounds())) {
			m_gameOver = true;
			break;
		}
	}

}

void Application::changeSnakeSpeed(int choice)
{
	if (choice == Speed::INCREASE) {

		if (m_snakeSpeed > 100) {
			m_snakeSpeed = 95;
		}

		m_snakeSpeed += 5;
	}

	else if (choice == Speed::DECREASE) {
		
		if (m_snakeSpeed < 10) {
			m_snakeSpeed = 10;
		}

		m_snakeSpeed -= 5;
	}
}

void Application::spawnApple()
{
	m_apple.setSize(sf::Vector2f(20.f, 20.f));
	m_apple.setFillColor(sf::Color::Red);
	m_apple.setPosition(sf::Vector2f(60.f, 60.f));
}

void Application::checkSnakeAppleCollision()
{
	bool badlocation = true;
	sf::Vector2i newSpawnPoint;

	//checks if new apple spawn location is not inside the snake
	while (badlocation)
	{
		badlocation = false;

		newSpawnPoint = { rand() % ((Globals::WINDOW_WIDTH - 20) / 20) * 20 , rand() % ((Globals::WINDOW_HEIGHT - 20) / 20) * 20 };

		for (int i = 1; i < m_snakeVec.size(); i++) {
			if (m_snakeVec[i].getSection().getGlobalBounds().intersects(sf::FloatRect(newSpawnPoint.x, newSpawnPoint.y, 20, 20))) {
				badlocation = true;
				break;
			}

		}
	}

	m_apple.setPosition(sf::Vector2f(newSpawnPoint.x, newSpawnPoint.y));
}

void Application::initWindow()
{
	window.create(sf::VideoMode(Globals::WINDOW_WIDTH, Globals::WINDOW_HEIGHT), Globals::WINDOW_TITLE , sf::Style::Close | sf::Style::Titlebar);

	window.setFramerateLimit(300);//You can change it to anything the game is framerate independent
}

void Application::initVariables()
{
	m_running  = true;
	m_paused   = false;
	m_gameOver = false;

	m_snakeSpeed = 20;
	m_snakeDirection = Direction::RIGHT;

	m_score = 0;

	if (!m_scoreFont.loadFromFile("../res/fonts/Roboto.ttf")) {
		std::cout << "Failed to load font\n";
	}

	m_scoreText.setFont(m_scoreFont);
	m_scoreText.setFillColor(sf::Color::Yellow);
	m_scoreText.setLetterSpacing(2);
	m_scoreText.setCharacterSize(16);

	m_GameOverText.setFont(m_scoreFont);
	m_GameOverText.setFillColor(sf::Color::Yellow);
	m_GameOverText.setLetterSpacing(3);
	m_GameOverText.setCharacterSize(60);
	m_GameOverText.setPosition(sf::Vector2f((Globals::WINDOW_WIDTH / 2) - (Globals::WINDOW_WIDTH / 6), Globals::WINDOW_HEIGHT / 3));

	TimeSinceLastMove = sf::Time::Zero;
}

void Application::quitGame()
{
	window.close();
	m_running = false;
}

void Application::HandleEvents()
{
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			quitGame();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			quitGame();
		}

		if (event.type == sf::Event::KeyPressed) {

			if (event.key.code == sf::Keyboard::P) {
				m_paused = !m_paused;
			}

		}

		if (event.type == sf::Event::KeyPressed && !m_paused)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				addDirection(Direction::UP);
				break;

			case sf::Keyboard::S:
				addDirection(Direction::DOWN);
				break;

			case sf::Keyboard::A:
				addDirection(Direction::LEFT);
				break;

			case sf::Keyboard::D:
				addDirection(Direction::RIGHT);
				break;

			case sf::Keyboard::Up:
				changeSnakeSpeed(Speed::INCREASE);
				break;
				
			case sf::Keyboard::Down:
				changeSnakeSpeed(Speed::DECREASE);
				break;
			}
		}
	}
}

void Application::Update()
{
	moveSnake();
	checkSnakeBounds();

	if (m_snakeVec[0].getSection().getGlobalBounds().intersects(m_apple.getGlobalBounds())) {
		checkSnakeAppleCollision();
		addNewSnakeSection();
		m_score += 10;
	}

	std::stringstream ss;
	ss << "Score : " << m_score;
	m_scoreText.setString(ss.str());

	for (auto& snakes : m_snakeVec)
	{
		snakes.update();
	}
}

void Application::Render()
{

	if (m_gameOver) {
		std::stringstream ss;

		ss << "Game Over!\nScore = " << m_score;
		m_GameOverText.setString(ss.str());

		window.draw(m_GameOverText);
	}

	else{
	
		for (auto& snakes : m_snakeVec)
		{
			window.draw(snakes.getSection());
		}

		window.draw(m_apple);
		window.draw(m_scoreText);
	}
}
