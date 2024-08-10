#include "Scene_Pause.h"
#include "../engine/GameEngine.h"
#include <iostream>
#include "../engine/Physics.h"

Scene_Pause::Scene_Pause(GameEngine* game) : Scene(game) {
	init();
}

void Scene_Pause::init() {
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::Escape, "CANCEL");

	m_menuPoints.push_back("Point1");
	m_menuPoints.push_back("Point2");
	m_menuPoints.push_back("Save");
	m_menuPoints.push_back("Quit");

	auto windowSize = m_game->getWindow().getSize();
	m_windowPos = Physics::getViewPosition(
		m_game->getWindow().getView(),
		Vec2(windowSize.x, windowSize.y));
}

void Scene_Pause::update() {
	sRender();
	m_currentFrame++;
}

void Scene_Pause::sRender() {
	m_game->getWindow().clear();

	float textY = 100;
	sf::Text menuText;
	menuText.setCharacterSize(30);
	menuText.setFont(m_game->getAssets().getFont("pixelmix"));
	menuText.setFillColor(sf::Color::White);
	for (std::string menuPoint : m_menuPoints) {
		menuText.setString(menuPoint);
		menuText.setPosition(m_windowPos.x + 70, m_windowPos.y + textY);
		m_game->getWindow().draw(menuText);
		textY += 40;
	}

	sf::RectangleShape cursor(sf::Vector2f(30, 30));
	cursor.setFillColor(sf::Color::Red);
	cursor.setPosition(m_windowPos.x + 20, m_windowPos.y + m_cursorPosY);
	m_game->getWindow().draw(cursor);

	m_game->getWindow().display();
}

void Scene_Pause::sDoAction(const Action& action) {
	float menuPointStep = 40;

	if (action.getName() == "UP" && action.getType() == "START") {
		if (m_menuIndex > 0) {
			m_cursorPosY -= menuPointStep;
			m_menuIndex--;
		}
	}
	if (action.getName() == "LEFT" && action.getType() == "START") {

	}
	if (action.getName() == "DOWN" && action.getType() == "START") {
		if (m_menuIndex < m_menuPoints.size() - 1) {
			m_cursorPosY += menuPointStep;
			m_menuIndex++;
		}
	}
	if (action.getName() == "RIGHT" && action.getType() == "START") {

	}
	if (action.getName() == "CANCEL" && action.getType() == "START") {
		onEnd();
	}
}

void Scene_Pause::onEnd() {
	m_game->changeScene("main", m_game->getScene("main"), true);
}
