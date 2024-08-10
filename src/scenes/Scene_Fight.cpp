#include "Scene_Fight.h"
#include "../engine/GameEngine.h"
#include "../engine/Physics.h"
#include <iostream>

Scene_Fight::Scene_Fight(GameEngine* game, std::shared_ptr<Entity>& player) : Scene(game) {
	m_player = player;
	init();
}

void Scene_Fight::init() {
	registerAction(sf::Keyboard::Enter, "DAMAGE");
	registerAction(sf::Keyboard::Escape, "QUIT");

	auto windowSize = m_game->getWindow().getSize();
	m_viewPosition = Physics::getViewPosition(
		m_game->getWindow().getView(), 
		Vec2(windowSize.x, windowSize.y));
}

void Scene_Fight::update() {
	sRender();

	m_currentFrame++;
}

void Scene_Fight::sRender() {
	m_game->getWindow().clear();
	
	sf::RectangleShape player;
	player.setTexture(&m_game->getAssets().getTexture("player_up"));
	player.setSize(sf::Vector2f(300, 300));
	player.setPosition(m_viewPosition.x + 150, m_viewPosition.y + 400);
	m_game->getWindow().draw(player);

	renderStats();

	m_game->getWindow().display();
}

void Scene_Fight::renderStats() {
	auto& playerStats = m_player->getComponent<CStats>();
	float HP_PERCENT = (playerStats.hp / playerStats.maxHp) * 100;
	float HP_BAR_WIDTH = (150 / 100) * HP_PERCENT;

	sf::RectangleShape playerHpBar;
	playerHpBar.setFillColor(sf::Color::Red);
	playerHpBar.setSize(sf::Vector2f(HP_BAR_WIDTH, 30));
	playerHpBar.setPosition(m_viewPosition.x + 400, m_viewPosition.y + 400);
	m_game->getWindow().draw(playerHpBar);

	sf::Text playerHpText;
	playerHpText.setCharacterSize(20);
	playerHpText.setFillColor(sf::Color::White);
	playerHpText.setFont(m_game->getAssets().getFont("pixelmix"));
	playerHpText.setPosition(m_viewPosition.x + 400, m_viewPosition.y + 450);

	int printHP = playerStats.hp;
	int printMaxHP = playerStats.maxHp;
	playerHpText.setString(
		std::string("HP: ") + 
		std::to_string(printHP) + 
		std::string(" / ")+ 
		std::to_string(printMaxHP));
	m_game->getWindow().draw(playerHpText);
}

void Scene_Fight::sDoAction(const Action& action) {
	if (action.getName() == "DAMAGE" && action.getType() == "START") {
		m_player->getComponent<CStats>().damage(8);
	}

	if (action.getName() == "QUIT" && action.getType() == "START") {
		onEnd();
	}
}

void Scene_Fight::onEnd() {
	m_game->changeScene("main", m_game->getScene("main"), true);
}