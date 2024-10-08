#include "Scene_Pause.h"
#include "../engine/GameEngine.h"
#include <iostream>
#include "../engine/Physics.h"

Scene_Pause::Scene_Pause(GameEngine* game) : Scene(game) {
	init();
}

void Scene_Pause::init() {
	registerAction(sf::Keyboard::Up, "UP");
	registerAction(sf::Keyboard::Down, "DOWN");
	registerAction(sf::Keyboard::Escape, "CANCEL");

	m_menuPoints.push_back("Point1");
	m_menuPoints.push_back("Point2");
	m_menuPoints.push_back("Save");
	m_menuPoints.push_back("Quit");

	auto windowSize = m_game->getWindow().getSize();
	Vec2 windowPos = Physics::getViewPosition(
		m_game->getWindow().getView(),
		Vec2(windowSize.x, windowSize.y));

	m_menuSystem = MenuSystem(
		m_game,
		Vec2(windowPos.x + 70, windowPos.y + 100),
		m_menuPoints,
		m_game->getAssets().getFont("pixelmix"));
}

void Scene_Pause::update() {
	sRender();
	m_currentFrame++;
}

void Scene_Pause::sRender() {
	m_game->getWindow().clear();
	m_menuSystem.render();
	m_game->getWindow().display();
}

void Scene_Pause::sDoAction(const Action& action) {
	if (action.getName() == "UP" && action.getType() == Action::START) {
		m_menuSystem.updateCursorBy(-1);
	}
	if (action.getName() == "DOWN" && action.getType() == Action::START) {
		m_menuSystem.updateCursorBy(1);
	}
	if (action.getName() == "CANCEL" && action.getType() == Action::START) {
		onEnd();
	}
}

void Scene_Pause::onEnd() {
	m_game->changeScene("main", m_game->getScene("main"), true);
}
