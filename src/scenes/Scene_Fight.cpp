#include "Scene_Fight.h"
#include "../engine/GameEngine.h"
#include "../engine/Physics.h"
#include <iostream>

Scene_Fight::Scene_Fight(GameEngine* game, std::shared_ptr<Entity>& player) : Scene(game) {
	m_player = player;
	init();
}

void Scene_Fight::init() {
	registerAction(sf::Keyboard::Enter, "SELECT");
	registerAction(sf::Keyboard::Up, "UP");
	registerAction(sf::Keyboard::Down, "DOWN");
	registerAction(sf::Keyboard::Space, "GET_DAMAGE");
	registerAction(sf::Keyboard::Escape, "QUIT");

	auto windowSize = m_game->getWindow().getSize();
	m_viewPosition = Physics::getViewPosition(
		m_game->getWindow().getView(), 
		Vec2(windowSize.x, windowSize.y));
	
	std::vector<std::string> attacks;
	attacks.push_back("Fire attack");
	attacks.push_back("Ice attack");
	attacks.push_back("Poison attack");
	attacks.push_back("Lightning attack");

	m_menu = MenuSystem(
		m_game, 
		Vec2(m_viewPosition.x + 800, m_viewPosition.y + 500), 
		attacks, 
		m_game->getAssets().getFont("pixelmix"));

	getEnemy();

	m_attack = m_entities.addEntity("attack");
	m_attack->addComponent<CTransform>();
	m_attack->getComponent<CTransform>().setPosition(m_viewPosition.x + 850, m_viewPosition.y + 50);
	m_attack->addComponent<CAnimation>();
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

	sf::RectangleShape enemy;
	enemy.setSize(sf::Vector2f(250, 250));
	enemy.setPosition(m_viewPosition.x + 850, m_viewPosition.y + 50);
	enemy.setFillColor(m_entities.getEntities("fight_enemy").at(0)->getComponent<CStats>().color);
	m_game->getWindow().draw(enemy);

	renderStats();
	m_menu.render();

	if (m_attackAnimationRunning) {
		renderAttackAnimation();
	}

	if (m_damagePointsAnimation.running) {
		renderDamagePointsAnimation(-3);
	}

	m_game->getWindow().display();
}

void Scene_Fight::renderStats() {
	auto& playerStats = m_player->getComponent<CStats>();
	renderHpBar(playerStats.hp, playerStats.maxHp, Vec2(m_viewPosition.x + 460, m_viewPosition.y + 400));
	renderHpText(playerStats.hp, playerStats.maxHp, Vec2(m_viewPosition.x + 465, m_viewPosition.y + 404));

	auto& enemyStats = m_entities.getEntities("fight_enemy").at(0)->getComponent<CStats>();
	renderHpBar(enemyStats.hp, enemyStats.maxHp, Vec2(m_viewPosition.x + 600, m_viewPosition.y + 100));
	renderHpText(enemyStats.hp, enemyStats.maxHp, Vec2(m_viewPosition.x + 605, m_viewPosition.y + 104));
}

void Scene_Fight::renderHpBar(float hp, float maxHp, const Vec2& pos) {
	float HP_PERCENT = (hp / maxHp) * 100;
	float HP_BAR_WIDTH = (200.0f / 100) * HP_PERCENT;

	sf::RectangleShape hpBar;
	hpBar.setFillColor(sf::Color::Red);
	hpBar.setSize(sf::Vector2f(HP_BAR_WIDTH, 30));
	hpBar.setPosition(pos.x, pos.y);
	m_game->getWindow().draw(hpBar);

	sf::RectangleShape hpBorder;
	hpBorder.setFillColor(sf::Color(0, 0, 0, 0));
	hpBorder.setOutlineThickness(2);
	hpBorder.setOutlineColor(sf::Color::White);
	hpBorder.setSize(sf::Vector2f(200, 30));
	hpBorder.setPosition(pos.x, pos.y);
	m_game->getWindow().draw(hpBorder);
}

void Scene_Fight::renderAttackAnimation() {
	auto& animation = m_attack->getComponent<CAnimation>();

	if (animation.animation.hasEnded()) {
		m_attackAnimationRunning = false;
	}
	else {
		sf::RectangleShape attack;
		auto& attackPos = m_attack->getComponent<CTransform>().getPos();
		attack.setPosition(attackPos.x, attackPos.y);
		attack.setSize(sf::Vector2f(250, 250));
		attack.setTexture(animation.animation.getSprite().getTexture());
		attack.setTextureRect(animation.animation.getSprite().getTextureRect());
		m_game->getWindow().draw(attack);

		animation.animation.update();
	}
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

void Scene_Fight::renderDamagePointsAnimation(int speed) {
	m_damagePointsAnimation.pos.x += speed;
	m_damagePointsAnimation.pos.y += m_damagePointsAnimation.gradient;
	m_damagePointsAnimation.opacity -= 3;
	if (m_damagePointsAnimation.opacity <= 0) {
		m_damagePointsAnimation.running = false;
		return;
	}

	sf::Text points(std::to_string(m_damagePointsAnimation.points), m_game->getAssets().getFont("pixelmix"), 40);
	points.setPosition(m_damagePointsAnimation.pos.x, m_damagePointsAnimation.pos.y);
	points.setFillColor(sf::Color(255, 255, 0, m_damagePointsAnimation.opacity));
	m_game->getWindow().draw(points);
}

void Scene_Fight::sDoAction(const Action& action) {
	if (action.getName() == "UP" && action.getType() == Action::START) {
		m_menu.updateCursorBy(-1);
	}

	if (action.getName() == "DOWN" && action.getType() == Action::START) {
		m_menu.updateCursorBy(1);
	}

	if (action.getName() == "SELECT" && action.getType() == Action::START) {
		auto& attackAnimation = m_attack->getComponent<CAnimation>();
		switch (m_menu.getIndex()) {
			case FIRE: 
				attack("fire_animation", 8); break;
			case ICE:
				attack("ice_animation", 5); break;
			case POISON:
				attack("poison_animation", 3); break;
			case LIGHTNING:
				attack("lightning_animation", 10); break;
		}
	}

	if (action.getName() == "GET_DAMAGE" && action.getType() == Action::START) {
		m_player->getComponent<CStats>().damage(8);
	}

	if (action.getName() == "QUIT" && action.getType() == Action::START) {
		onEnd();
	}
}

void Scene_Fight::attack(const std::string& animationName, int damage) {
	if (!m_attackAnimationRunning) {
		m_attack->getComponent<CAnimation>().animation = m_game->getAssets().getAnimation(animationName);
		m_entities.getEntities("fight_enemy").at(0)->getComponent<CStats>().damage(damage);
		fillDamagePointsAnimation(damage);
		m_attackAnimationRunning = true;
	}
}

void Scene_Fight::fillDamagePointsAnimation(int damage) {
	m_damagePointsAnimation.running = true;
	float randGradient = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2);
	m_damagePointsAnimation.gradient = rand() % 2 == 0 ? randGradient : -randGradient;
	m_damagePointsAnimation.opacity = 255;
	m_damagePointsAnimation.points = damage;
	m_damagePointsAnimation.pos = Vec2(m_viewPosition.x + 965, m_viewPosition.y + 165);
}

void Scene_Fight::onEnd() {
	m_game->changeScene("main", m_game->getScene("main"), true);
}