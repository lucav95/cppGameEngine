#include "MenuSystem.h"
#include <iostream>

MenuSystem::MenuSystem() {}

MenuSystem::MenuSystem(
	GameEngine* game, 
	const Vec2& pos, 
	const std::vector<std::string>& points, 
	const sf::Font& font) : 
	m_game(game), m_pos(pos), m_menuPoints(points), m_font(font) { }

void MenuSystem::render() {

	float textY = 0;
	sf::Text text;
	text.setCharacterSize(30);
	text.setFont(m_font);
	text.setFillColor(sf::Color::White);
	text.setScale(m_scale.x, m_scale.y);
	for (auto point : m_menuPoints) {
		text.setString(point);
		text.setPosition(m_pos.x + 70, m_pos.y + textY);
		m_game->getWindow().draw(text);
		textY += (m_scale.y * 40);
	}
	sf::RectangleShape cursor(sf::Vector2f(30, 30));
	cursor.setTexture(&m_game->getAssets().getTexture("menu_cursor"));
	cursor.setPosition(m_pos.x + 20, m_pos.y + m_cursorPosY);
	cursor.setScale(m_scale.x, m_scale.y);
	m_game->getWindow().draw(cursor);
}

void MenuSystem::setPosition(const Vec2& pos) {
	m_pos = pos;
}

void MenuSystem::setMenuPoints(const std::vector<std::string>& points) {
	m_menuPoints = points;
}

void MenuSystem::setFont(const sf::Font& font) {
	m_font = font;
}

void MenuSystem::setScale(const Vec2& scale) {
	m_scale = scale;
	m_stepPixels = m_scale.y * 40;
}

int MenuSystem::getIndex() {
	return m_index;
}

void MenuSystem::updateCursorBy(int points) {
	if ((points < 0 && m_index == 0) || (points > 0 && m_index == m_menuPoints.size() - 1)) {
		return;
	}
	m_index += points;
	m_cursorPosY = m_index * m_stepPixels;
}