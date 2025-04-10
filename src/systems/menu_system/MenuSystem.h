#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "../../engine/Vec2.h"
#include "../../engine/GameEngine.h"
#include "../InventorySystem.h"

template <typename T>
class MenuSystem {
	GameEngine*					m_game = nullptr;
	std::vector<T>				m_menuPoints;
	Vec2						m_pos;
	Vec2						m_scale = Vec2(1.0, 1.0);
	float						m_cursorPosY = 0;
	sf::Font					m_font;
	int							m_index = 0;
	int							m_stepPixels = 40;

public:
	MenuSystem() { }

	MenuSystem(
		GameEngine* game,
		const Vec2& pos,
		const std::vector<T>& points,
		const sf::Font& font) :
		m_game(game), m_pos(pos), m_menuPoints(points), m_font(font) { }


	void render() {
		float textY = 0;
		sf::Text text;
		text.setCharacterSize(30);
		text.setFont(m_font);
		text.setFillColor(sf::Color::White);
		text.setScale(m_scale.x, m_scale.y);
		
		for (auto& point : m_menuPoints) {
			// constexpr evaluation at compile time
			if constexpr (std::is_same<T, std::string>::value) {
				text.setString(point);
			}
			if constexpr (std::is_same<T, InventorySystem::Item>::value) {
				text.setString(std::to_string(point.count) + "x " + point.name);
			}
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

	void setMenuPoints(const std::vector<T>& points) {
		m_menuPoints = points;
	}

	void updateCursorBy(int points) {
		if ((points < 0 && m_index == 0) || (points > 0 && m_index == m_menuPoints.size() - 1)) {
			return;
		}
		m_index += points;
		m_cursorPosY = m_index * m_stepPixels;
	}

	void setPosition(const Vec2& pos) {
		m_pos = pos;
	}

	void setFont(const sf::Font& font) {
		m_font = font;
	}

	void setScale(const Vec2& scale) {
		m_scale = scale;
		m_stepPixels = m_scale.y * 40;
	}

	int	getIndex() {
		return m_index;
	}

	T getCurrentPoint() {
		return m_menuPoints[m_index];
	}
};