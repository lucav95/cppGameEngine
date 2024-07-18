#include "Scene_Pause.h"
#include "../engine/GameEngine.h"
#include <iostream>

Scene_Pause::Scene_Pause(GameEngine* game) : Scene(game) {
	init();
}

void Scene_Pause::init() {
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::Escape, "CANCEL");

	m_camera = sf::View(
		sf::Vector2f(m_game->getWindow().getSize().x / 2, m_game->getWindow().getSize().y / 2),
		sf::Vector2f(m_game->getWindow().getSize().x, m_game->getWindow().getSize().y));
	m_game->getWindow().setView(m_camera);
}

void Scene_Pause::update() {
	sRender();
	m_currentFrame++;
}

void Scene_Pause::sRender() {
	m_game->getWindow().clear();
	m_game->getWindow().setView(m_camera);

	sf::RectangleShape background;
	background.setFillColor(sf::Color::Red);
	background.setSize(sf::Vector2f(m_game->getWindow().getSize().x, m_game->getWindow().getSize().y));
	background.setPosition(0, 0);
	m_game->getWindow().draw(background);

	m_game->getWindow().display();
}

void Scene_Pause::sDoAction(const Action& action) {
	if (action.getName() == "UP" && action.getType() == "START") {
		
	}
	if (action.getName() == "LEFT" && action.getType() == "START") {

	}
	if (action.getName() == "DOWN" && action.getType() == "START") {

	}
	if (action.getName() == "RIGHT" && action.getType() == "START") {

	}
	if (action.getName() == "CANCEL" && action.getType() == "START") {
		std::cout << "cancel\n";
		onEnd();
	}
}

void Scene_Pause::onEnd() {
	// camera to player
	m_game->changeScene("main", m_game->getScene("main"));
}