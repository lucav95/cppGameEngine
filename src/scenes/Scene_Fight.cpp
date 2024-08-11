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
	registerAction(sf::Keyboard::Space, "GET_DAMAGE");
	registerAction(sf::Keyboard::Escape, "QUIT");

	auto windowSize = m_game->getWindow().getSize();
	m_viewPosition = Physics::getViewPosition(
		m_game->getWindow().getView(), 
		Vec2(windowSize.x, windowSize.y));

	getEnemy();
}

void Scene_Fight::getEnemy() {
	auto enemy = m_entities.addEntity("fight_enemy");

	int random = (std::rand() % 2) + 1;
	switch (random) {
		case 1: 
			enemy->addComponent<CStats>(120, 120, sf::Color::Red);
			break;
		case 2: 
			enemy->addComponent<CStats>(90, 90, sf::Color::Blue);
			break;
	}
}

void Scene_Fight::update() {
	m_entities.update();
	sRender();

	if (m_entities.getEntities("fight_enemy").at(0)->getComponent<CStats>().hp == 0) {
		onEnd();
	}

	m_currentFrame++;
}

void Scene_Fight::sRender() {
	m_game->getWindow().clear();
	
	sf::RectangleShape player;
	player.setTexture(&m_game->getAssets().getTexture("player_up"));
	player.setSize(sf::Vector2f(300, 300));
	player.setPosition(m_viewPosition.x + 150, m_viewPosition.y + 400);
	m_game->getWindow().draw(player);

	sf::RectangleShape enemyRect;
	enemyRect.setSize(sf::Vector2f(250, 250));
	enemyRect.setPosition(m_viewPosition.x + 800, m_viewPosition.y + 50);
	enemyRect.setFillColor(m_entities.getEntities("fight_enemy").at(0)->getComponent<CStats>().color);
	m_game->getWindow().draw(enemyRect);

	renderStats();

	m_game->getWindow().display();
}

void Scene_Fight::renderStats() {
	auto& playerStats = m_player->getComponent<CStats>();
	renderHpBar(playerStats.hp, playerStats.maxHp, Vec2(m_viewPosition.x + 400, m_viewPosition.y + 400));
	renderHpText(playerStats.hp, playerStats.maxHp, Vec2(m_viewPosition.x + 400, m_viewPosition.y + 450));

	auto& enemyStats = m_entities.getEntities("fight_enemy").at(0)->getComponent<CStats>();
	renderHpBar(enemyStats.hp, enemyStats.maxHp, Vec2(m_viewPosition.x + 550, m_viewPosition.y + 100));
	renderHpText(enemyStats.hp, enemyStats.maxHp, Vec2(m_viewPosition.x + 550, m_viewPosition.y + 150));
}

void Scene_Fight::renderHpBar(float hp, float maxHp, const Vec2& pos) {
	float HP_PERCENT = (hp / maxHp) * 100;
	float HP_BAR_WIDTH = (200 / 100) * HP_PERCENT;

	sf::RectangleShape hpBar;
	hpBar.setFillColor(sf::Color::Red);
	hpBar.setSize(sf::Vector2f(HP_BAR_WIDTH, 30));
	hpBar.setPosition(pos.x, pos.y);
	m_game->getWindow().draw(hpBar);
}

void Scene_Fight::renderHpText(int hp, int maxHp, const Vec2& pos) {
	sf::Text hpText;
	hpText.setCharacterSize(20);
	hpText.setFillColor(sf::Color::White);
	hpText.setFont(m_game->getAssets().getFont("pixelmix"));
	hpText.setPosition(pos.x, pos.y);

	hpText.setString(
		std::string("HP: ") +
		std::to_string(hp) +
		std::string(" / ") +
		std::to_string(maxHp));
	m_game->getWindow().draw(hpText);
}

void Scene_Fight::sDoAction(const Action& action) {
	if (action.getName() == "DAMAGE" && action.getType() == "START") {
		m_entities.getEntities("fight_enemy").at(0)->getComponent<CStats>().damage(8);
	}

	if (action.getName() == "GET_DAMAGE" && action.getType() == "START") {
		m_player->getComponent<CStats>().damage(8);
	}

	if (action.getName() == "QUIT" && action.getType() == "START") {
		onEnd();
	}
}

void Scene_Fight::onEnd() {
	m_entities.getEntities("fight_enemy").at(0)->destroy();
	m_game->changeScene("main", m_game->getScene("main"), true);
}