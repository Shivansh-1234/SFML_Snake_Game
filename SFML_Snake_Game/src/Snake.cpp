#include "..\headers\Snake.h"

Snake::Snake(Vec2 startpos)
{
	m_section.setSize(sf::Vector2f(20.f, 20.f));
	m_section.setFillColor(sf::Color::White);
	m_section.setPosition(sf::Vector2f(startpos.x, startpos.y));

	m_position = startpos;
}

Snake::~Snake()
{
}

void Snake::setSnakePos(Vec2 startpos)
{
	m_position = startpos;
}

Vec2& Snake::getSnakePos()
{
	return m_position;
}

sf::RectangleShape& Snake::getSection()
{
	return m_section;
}

void Snake::update()
{
	m_section.setPosition(sf::Vector2f(m_position.x, m_position.y));
}
