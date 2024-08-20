#include "TextBoxSystem.h"
#include "../../engine/Physics.h"
#include <sstream>
#include <iostream>

TextBoxSystem::TextBoxSystem(GameEngine* game) : m_game(game) { }

void TextBoxSystem::render(int currentFrame) {
	const auto WINDOW_SIZE = m_game->getWindow().getSize();

	const Vec2 VIEW_POSITION = Physics::getViewPosition(
		m_game->getWindow().getView(),
		Vec2(WINDOW_SIZE.x, WINDOW_SIZE.y));
	const Vec2 BOX_SIZE(1000, 200);

	sf::RectangleShape box(sf::Vector2f(BOX_SIZE.x, BOX_SIZE.y));
	box.setFillColor(sf::Color::Black);
	box.setOutlineColor(sf::Color::White);
	box.setOutlineThickness(10);
	box.setPosition(VIEW_POSITION.x + 140, VIEW_POSITION.y + 500);
	m_game->getWindow().draw(box);

	sf::Text renderText(m_boxes.at(m_currentBox), m_game->getAssets().getFont("pixelmix"), 30);
	renderText.setFillColor(sf::Color::White);
	renderText.setLineSpacing(1.4);
	renderText.setPosition(VIEW_POSITION.x + 160, VIEW_POSITION.y + 520);
	m_game->getWindow().draw(renderText);

	m_dialogArrowShowing = currentFrame % 30 == 0 ? !m_dialogArrowShowing : m_dialogArrowShowing;
	
	if (m_dialogArrowShowing && m_currentBox < m_boxes.size() - 1) {
		sf::RectangleShape dialogArrow(sf::Vector2f(30, 30));
		dialogArrow.setTexture(&m_game->getAssets().getTexture("text_arrow"));
		dialogArrow.setPosition(VIEW_POSITION.x + 1100, VIEW_POSITION.y + 660);
		m_game->getWindow().draw(dialogArrow);
	}
}

void TextBoxSystem::setBoxes() {
	m_boxes.clear();
	sf::Text boxText("", m_game->getAssets().getFont("pixelmix"), 30);

	std::stringstream ss(m_text);
	std::string token;
	std::string formatted = "";
	int lineCounter = 1;
	
	int BOX_WIDTH = 1000;

	while (std::getline(ss, token, ' ')) {
		boxText.setString(boxText.getString() + (" " + token));
		if (boxText.getLocalBounds().width >= BOX_WIDTH) {
			lineCounter++;
			boxText.setString(token);
			if (lineCounter > 4) {
				m_boxes.push_back(formatted);
				formatted = token;
				lineCounter = 1;
			}
			else {
				formatted += ("\n" + token);
			}
			continue;
		}
		formatted += formatted.empty() ? token : (" " + token);
	}
	m_boxes.push_back(formatted);
}

int TextBoxSystem::getBoxCount() {
	return m_boxes.size();
}

void TextBoxSystem::setText(const std::string& text) {
	m_text = text;
	setBoxes();
}

const std::string& TextBoxSystem::getText() const {
	return m_text;
}

void TextBoxSystem::setCurrentBox(int currentBox) {
	m_currentBox = currentBox;
}

int TextBoxSystem::getCurrentBox() {
	return m_currentBox;
}