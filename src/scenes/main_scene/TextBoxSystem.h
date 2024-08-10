#pragma once

#include <SFML/Graphics.hpp>
#include "../../engine/GameEngine.h"

class TextBoxSystem {
	bool						m_dialogArrowShowing = false;
	std::string					m_text;
	GameEngine*					m_game;
	std::vector<std::string>	m_boxes;
	int							m_currentBox = 0;

	void						setBoxes();

public:
	TextBoxSystem(GameEngine* game);

	void						render(int currentFrame);
	int							getBoxCount();
	void						setText(const std::string& text);
	const std::string&			getText() const;
	void						setCurrentBox(int currentBox);
	int							getCurrentBox();
};