#include "TextBoxSystem.h"
#include "../../engine/Physics.h"
#include <sstream>

TextBoxSystem::TextBoxSystem(GameEngine* game) : m_game(game) { }

void TextBoxSystem::render(int currentFrame) {
	auto WINDOW_SIZE = m_game->getWindow().getSize();

	Vec2 VIEW_POSITION = Physics::getViewPosition(
		m_game->getWindow().getView(),
		Vec2(WINDOW_SIZE.x, WINDOW_SIZE.y));

	Vec2 BOX_SIZE(1000, 200);

	sf::RectangleShape box(sf::Vector2f(BOX_SIZE.x, BOX_SIZE.y));
	box.setFillColor(sf::Color::Black);
	box.setOutlineColor(sf::Color::White);
	box.setOutlineThickness(10);
	box.setPosition(VIEW_POSITION.x + 140, VIEW_POSITION.y + 500);
	m_game->getWindow().draw(box);

	sf::Text renderText;
	renderText.setFillColor(sf::Color::White);
	renderText.setCharacterSize(30);
	renderText.setFont(m_game->getAssets().getFont("pixelmix"));

	std::stringstream ss(m_text);
	std::string token;
	std::string formatted = "";
	while (std::getline(ss, token, ' ')) {
		renderText.setString(renderText.getString() + (" " + token));
		if (renderText.getLocalBounds().width >= BOX_SIZE.x) {
			renderText.setString(token);
			formatted += ("\n" + token);
			continue;
		}
		formatted += formatted.empty() ? token : (" " + token);
	}
	renderText.setString(formatted);
	renderText.setLineSpacing(1.4);
	renderText.setPosition(VIEW_POSITION.x + 160, VIEW_POSITION.y + 520);
	m_game->getWindow().draw(renderText);

	m_dialogArrowShowing = currentFrame % 30 == 0 ? !m_dialogArrowShowing : m_dialogArrowShowing;

	if (m_dialogArrowShowing) {
		sf::RectangleShape dialogArrow(sf::Vector2f(30, 30));
		dialogArrow.setTexture(&m_game->getAssets().getTexture("text_arrow"));
		dialogArrow.setPosition(VIEW_POSITION.x + 1100, VIEW_POSITION.y + 660);
		m_game->getWindow().draw(dialogArrow);
	}
}

void TextBoxSystem::setText(const std::string& text) {
	m_text = text;
}

const std::string& TextBoxSystem::getText() const {
	return m_text;
}