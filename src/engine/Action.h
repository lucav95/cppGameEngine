#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class Action {
public:
	enum Type {
		NONE = 0, START, END
	};

	Action();
	Action(const std::string& name, const Type type);
	Action(const std::string& name, const Type type, const sf::Vector2i& mousePos);


	const std::string&	getName() const;
	const Type	getType() const;
	const sf::Vector2i& getMousePos() const;

private:
	std::string			m_name = "NONE";
	sf::Vector2i		m_mousePos = sf::Vector2i(0, 0);
	Type				m_type = Type::NONE;
};
