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
	for (auto& attack : m_player->getComponent<CStats>().attacks) {
		attacks.push_back(attack.name);
	}

	m_menu = MenuSystem(
		m_game, 
		Vec2(m_viewPosition.x + 800, m_viewPosition.y + 500), 
		attacks, 
		m_game->getAssets().getFont("pixelmix"));

	getEnemy();

	m_attack = m_entities.addEntity("attack");
	m_attack->addComponent<CTransform>().setPosition(m_viewPosition.x + 850, m_viewPosition.y + 50);
	m_attack->addComponent<CGraphics>();
}

void Scene_Fight::getEnemy() {
	int random = (std::rand() % 3) + 1; // 1 - 3
	m_enemy = m_entities.addEntity("fight_enemy");

	switch (random) {
		case 1: 
			m_enemy->addComponent<CGraphics>("fire_enemy");
			m_enemy->addComponent<CStats>(110, 110, CStats::ICE, CStats::LIGHTNING);
			break;
		case 2: 
			m_enemy->addComponent<CGraphics>("tree_enemy");
			m_enemy->addComponent<CStats>(150, 150, CStats::FIRE, CStats::POISON);
			break;
		case 3:
			m_enemy->addComponent<CGraphics>("water_enemy");
			m_enemy->addComponent<CStats>(90, 90, CStats::LIGHTNING, CStats::FIRE);
			break;
	}
	m_enemy->addComponent<CState>(CState::NONE);
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
	
	sf::RectangleShape player(sf::Vector2f(300, 300));
	player.setTexture(&m_game->getAssets().getTexture("player_up"));
	player.setPosition(m_viewPosition.x + 150, m_viewPosition.y + 400);
	m_game->getWindow().draw(player);

	sf::RectangleShape enemy(sf::Vector2f(250, 250));
	enemy.setPosition(m_viewPosition.x + 850, m_viewPosition.y + 50);
	enemy.setTexture(&m_game->getAssets().getTexture(m_enemy->getComponent<CGraphics>().texture));
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

	auto& enemyStats = m_enemy->getComponent<CStats>();
	renderHpBar(enemyStats.hp, enemyStats.maxHp, Vec2(m_viewPosition.x + 600, m_viewPosition.y + 100));
	renderHpText(enemyStats.hp, enemyStats.maxHp, Vec2(m_viewPosition.x + 605, m_viewPosition.y + 104));
}

void Scene_Fight::renderHpBar(float hp, float maxHp, const Vec2& pos) {
	float HP_PERCENT = (hp / maxHp) * 100;
	float HP_BAR_WIDTH = (200.0f / 100) * HP_PERCENT;

	sf::RectangleShape hpBar(sf::Vector2f(HP_BAR_WIDTH, 30));
	hpBar.setFillColor(sf::Color::Red);
	hpBar.setPosition(pos.x, pos.y);
	m_game->getWindow().draw(hpBar);

	sf::RectangleShape hpBorder(sf::Vector2f(200, 30));
	hpBorder.setFillColor(sf::Color(0, 0, 0, 0));
	hpBorder.setOutlineThickness(2);
	hpBorder.setOutlineColor(sf::Color::White);
	hpBorder.setPosition(pos.x, pos.y);
	m_game->getWindow().draw(hpBorder);
}

void Scene_Fight::renderAttackAnimation() {
	auto& graphics = m_attack->getComponent<CGraphics>();

	if (graphics.animation.hasEnded()) {
		m_attackAnimationRunning = false;
	}
	else {
		sf::RectangleShape attack(sf::Vector2f(250, 250));
		auto& attackPos = m_attack->getComponent<CTransform>().getPos();
		attack.setPosition(attackPos.x, attackPos.y);
		attack.setTexture(graphics.animation.getSprite().getTexture());
		attack.setTextureRect(graphics.animation.getSprite().getTextureRect());
		m_game->getWindow().draw(attack);

		graphics.animation.update();
	}
}

void Scene_Fight::renderHpText(int hp, int maxHp, const Vec2& pos) {
	sf::Text hpText(
		std::string("HP: ") +
		std::to_string(hp) +
		std::string(" / ") +
		std::to_string(maxHp),
		m_game->getAssets().getFont("pixelmix"), 20);
	hpText.setFillColor(sf::Color::White);
	hpText.setPosition(pos.x, pos.y);
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
		auto& attackAnimation = m_attack->getComponent<CGraphics>();
		switch (m_menu.getIndex()) {
			case FIRE:		attack("fire_animation", "Fire"); break;
			case ICE:		attack("ice_animation", "Ice"); break;
			case POISON:	attack("poison_animation", "Poison"); break;
			case LIGHTNING:	attack("lightning_animation", "Lightning"); break;
		}
	}

	if (action.getName() == "GET_DAMAGE" && action.getType() == Action::START) {
		m_player->getComponent<CStats>().damage(8);
	}

	if (action.getName() == "QUIT" && action.getType() == Action::START) {
		onEnd();
	}
}

void Scene_Fight::attack(const std::string& animationName, const std::string& attackName) {
	if (!m_attackAnimationRunning) {
		m_attack->getComponent<CGraphics>().animation = m_game->getAssets().getAnimation(animationName);
		int damage = m_enemy->getComponent<CStats>().damage(m_player->getComponent<CStats>().getAttack(attackName));
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