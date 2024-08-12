#include "Action.h"

Action::Action() {};

Action::Action(const std::string& name, const Type type) 
	: m_name(name), m_type(type) { }

Action::Action(const std::string& name, const Type type, const sf::Vector2i& mousePos)
	: m_name(name), m_type(type), m_mousePos(mousePos) { }

const std::string& Action::getName() const {
	return m_name;
}

const Action::Type Action::getType() const {
	return m_type;
}

const sf::Vector2i& Action::getMousePos() const {
	return m_mousePos;
}
