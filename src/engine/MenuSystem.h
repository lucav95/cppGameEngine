#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "../engine/GameEngine.h"

class MenuSystem {
	GameEngine*					m_game = nullptr;
	std::vector<std::string>	m_menuPoints;
	Vec2						m_pos;
	Vec2						m_scale = Vec2(1.0, 1.0);
	float						m_cursorPosY = 0;
	sf::Font					m_font;
	int							m_index = 0;
	int							m_stepPixels = 40;

public:
	MenuSystem();
	MenuSystem(
		GameEngine* game,
		const Vec2& pos, 
		const std::vector<std::string>& points, 
		const sf::Font& font);

	void						render();
	void						setPosition(const Vec2& pos);
	void						setMenuPoints(const std::vector<std::string>& points);
	void						setFont(const sf::Font& font);
	void						setScale(const Vec2& scale);
	int							getIndex();
	void						updateCursorBy(int points);
};