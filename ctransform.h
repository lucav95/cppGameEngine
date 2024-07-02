#include <SFML/Graphics.hpp>

class CTransform {
public:
	sf::Vector2f m_pos;
	sf::Vector2f m_velocity;
	CTransform() {}
	CTransform(const sf::Vector2f& p, const sf::Vector2f& v) : m_pos(p), m_velocity(v) {}
};
