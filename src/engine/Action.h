#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class Action {
	std::string			m_name = "NONE";
	std::string			m_type = "NONE"; // "START" / "END"
	sf::Vector2i		m_mousePos = sf::Vector2i(0, 0);

public:
	Action();
	Action(const std::string& name, const std::string& type);
	Action(const std::string& name, const std::string& type, const sf::Vector2i& mousePos);

	const std::string&	getName() const;
	const std::string&	getType() const;
	const sf::Vector2i& getMousePos() const;
};