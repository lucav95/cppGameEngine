#include "Animation.h"
#include <iostream>

Animation::Animation() {}

Animation::Animation(const std::string& name, const sf::Texture& t) : m_name(name) {
	m_frameSize = {t.getSize().x / (float)m_frameCount, (float)t.getSize().y};
	m_sprite.setTexture(t);
	m_sprite.setTextureRect(sf::IntRect(0, 0, t.getSize().x, t.getSize().y));
}

Animation::Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed)
	: m_name(name), m_frameCount(frameCount), m_speed(speed) {
	m_frameSize = { t.getSize().x / (float)frameCount, (float)t.getSize().y };
	m_sprite.setTexture(t);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_frameSize.x, m_frameSize.y));
}

void Animation::update() {
	m_aliveDuration++;
	m_currentFrame = (m_aliveDuration / m_speed) % m_frameCount;
	m_sprite.setTextureRect(sf::IntRect((m_currentFrame * m_frameSize.x), 0, m_frameSize.x, m_frameSize.y));
}

const std::string& Animation::getName() const {
	return m_name;
}

const Vec2& Animation::getSize() const {
	return m_frameSize;
}

const sf::Sprite& Animation::getSprite() const {
	return m_sprite;
}