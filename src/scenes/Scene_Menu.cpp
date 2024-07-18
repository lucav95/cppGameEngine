#include "Scene_Menu.h"
#include "Scene_Main.h"
#include "../engine/GameEngine.h"
#include <iostream>

Scene_Menu::Scene_Menu(GameEngine* game) : Scene(game) {
	init();
}

void Scene_Menu::init() {
	m_start.x = 20;
	m_start.y = 200;
	m_start.w = 150;
	m_start.h = 45;
}

void Scene_Menu::update() {
	sRender();
	checkHover();
	m_currentFrame++;
}

void Scene_Menu::sRender() {
	m_game->getWindow().clear();
	
	sf::Text heading;
	heading.setFont(m_game->getAssets().getFont("pixelmix"));
	heading.setString("Start");
	heading.setCharacterSize(45);
	heading.setFillColor(m_startHover ? sf::Color::Green : sf::Color::White);
	heading.setPosition(m_start.x, m_start.y);
	m_game->getWindow().draw(heading);

	m_game->getWindow().display();
}

void Scene_Menu::checkHover() {
	sf::Vector2i mousePos = sf::Mouse::getPosition(m_game->getWindow());
	m_startHover =
		mousePos.x >= m_start.x &&
		mousePos.x <= m_start.x + m_start.w &&
		mousePos.y >= m_start.y &&
		mousePos.y <= m_start.y + m_start.h;
}

void Scene_Menu::sDoAction(const Action& action) {

	if (action.getName() == "MOUSE_LEFT" && action.getType() == "START" && m_startHover) {
		onEnd();
	}
}

void Scene_Menu::onEnd() {
	m_game->changeScene("main", std::make_shared<Scene_Main>(m_game), true);
}
