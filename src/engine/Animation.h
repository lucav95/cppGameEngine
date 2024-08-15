#pragma once

#include "Vec2.h"
#include <SFML/Graphics.hpp>

class Animation {
	sf::Sprite			m_sprite;
	size_t				m_frameCount = 1;
	size_t				m_currentFrame = 0;
	size_t				m_lastFrame = 0;
	Vec2				m_frameSize = { 1.0f, 1.0f };
	size_t				m_speed = 0; // frame delay
	size_t				m_aliveDuration = 0;
	std::string			m_name;

public:
	Animation();
	Animation(const std::string& name, const sf::Texture& t);
	Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed);

	void				update();
	void				reset();
	bool				hasEnded() const;
	const std::string&	getName() const;
	const Vec2&			getSize() const;
	const sf::Sprite&	getSprite() const;
};