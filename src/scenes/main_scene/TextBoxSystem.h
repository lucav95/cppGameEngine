#pragma once

#include <SFML/Graphics.hpp>
#include "../../engine/GameEngine.h"

class TextBoxSystem {
	bool				m_dialogArrowShowing = false;
	std::string			m_text;
	GameEngine*			m_game;

public:
	TextBoxSystem(GameEngine* game);

	void				render(int currentFrame);
	void				setText(const std::string& text);
	const std::string&	getText() const;
};