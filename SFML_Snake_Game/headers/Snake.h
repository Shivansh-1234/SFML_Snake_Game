#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Vec2.h"

class Snake
{
public:

	Snake(Vec2 startpos);
	~Snake();

	void setSnakePos(Vec2 startpos);
	Vec2& getSnakePos();
	sf::RectangleShape& getSection();

	void update();
private:

	Vec2 m_position;
	sf::RectangleShape m_section;
};
