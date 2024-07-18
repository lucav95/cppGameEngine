#include "Action.h"

Action::Action() {};

Action::Action(const std::string& name, const std::string& type) 
	: m_name(name), m_type(type) { }

Action::Action(const std::string& name, const std::string& type, const sf::Vector2i& mousePos)
	: m_name(name), m_type(type), m_mousePos(mousePos) { }

const std::string& Action::getName() const {
	return m_name;
}

const std::string& Action::getType() const {
	return m_type;
}

std::string Action::toString() const {
	return m_name + ", " + m_type;
}