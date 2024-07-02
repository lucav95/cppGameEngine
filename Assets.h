#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include "Animation.h"

typedef std::map<std::string, sf::Texture>	TextureMap;
typedef std::map<std::string, Animation>	AnimationMap;
typedef std::map<std::string, sf::Sound>	SoundMap;
typedef std::map<std::string, sf::Font>		FontMap;

class Assets {
	TextureMap				m_textures;
	AnimationMap			m_animations;
	SoundMap				m_sounds;
	FontMap					m_fonts;

public:
	void					addTexture(const std::string& name, const std::string& path);
	void					addAnimation(const std::string& name, const Animation& animation);
	void					addSound(const std::string& name, const std::string& path);
	void					addFont(const std::string& name, const std::string& path);

	const sf::Texture&		getTexture(const std::string& name) const;
	Animation&				getAnimation(const std::string& name);
	const sf::Sound&		getSound(const std::string& name) const;
	const sf::Font&			getFont(const std::string& name) const;
};