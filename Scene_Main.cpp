#include "Scene_Main.h"
#include "GameEngine.h"
#include <iostream>

Scene_Main::Scene_Main(GameEngine* game)
	: Scene(game) {
	init();
}

void Scene_Main::init() {
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::Escape, "PAUSE");

	spawnEnemy(300.0f, 300.0f);
	spawnEnemy(300.0f, 464.0f);
	spawnPlayer();
}

void Scene_Main::update() {
	if (m_paused) {
		return;
	}
	m_entities.update();
	//sEnemySpawner();
	sMovement();
	sCollision();
	sRender();

	// git test
	// overflow in like 400 days?
	m_currentFrame++;
}

void Scene_Main::sRender() {
	m_game->getWindow().clear();
	for (auto e : m_entities.getEntities()) {

		auto& entityBoundingBox = e->getComponent<CBoundingBox>();
		auto& entityTransform = e->getComponent<CTransform>();
		
		if (e->getTag() != "player") {
			sf::RectangleShape rect;
			rect.setFillColor(sf::Color(255, 0, 0));
			rect.setOutlineColor(sf::Color(0, 0, 255));
			rect.setPosition(sf::Vector2f(
				entityTransform.pos.x - entityBoundingBox.halfSize.x,
				entityTransform.pos.y - entityBoundingBox.halfSize.y));
			rect.setSize(sf::Vector2f(entityBoundingBox.size.x, entityBoundingBox.size.y));
			m_game->getWindow().draw(rect);

			sf::RectangleShape point;
			point.setFillColor(sf::Color(0, 255, 0));
			point.setPosition(sf::Vector2f(entityTransform.pos.x -1, entityTransform.pos.y -1));
			point.setSize(sf::Vector2f(3, 3));
			m_game->getWindow().draw(point);
		}
		else {

			auto& playerInput = m_player->getComponent<CInput>();

			sf::RectangleShape player(sf::Vector2f(80, 80));
			if (playerInput.up || playerInput.down || playerInput.left || playerInput.right) {
				player.setTexture(m_game->getAssets().getAnimation(m_playerAnimation).getSprite().getTexture());
				player.setTextureRect(m_game->getAssets().getAnimation(m_playerAnimation).getSprite().getTextureRect());
				m_game->getAssets().getAnimation(m_playerAnimation).update();
			}
			else {
				player.setTexture(&m_game->getAssets().getTexture(m_playerStandingTexture));
			}
			player.setPosition(
				m_player->getComponent<CTransform>().pos.x - m_player->getComponent<CBoundingBox>().halfSize.x,
				m_player->getComponent<CTransform>().pos.y - m_player->getComponent<CBoundingBox>().halfSize.y);
			m_game->getWindow().draw(player);
		}
		
	}
	m_game->getWindow().display();
}

void Scene_Main::sCollision() {

	auto& playerTransform = m_player->getComponent<CTransform>();

	for (auto enemy : m_entities.getEntities("enemy")) {

		auto& enemyTransform = enemy->getComponent<CTransform>();

		Vec2 delta = Vec2(
			abs(playerTransform.pos.x - enemyTransform.pos.x),
			abs(playerTransform.pos.y - enemyTransform.pos.y));
		float xOverlap = m_player->getComponent<CBoundingBox>().halfSize.x + enemy->getComponent<CBoundingBox>().halfSize.x - delta.x;
		float yOverlap = m_player->getComponent<CBoundingBox>().halfSize.y + enemy->getComponent<CBoundingBox>().halfSize.y - delta.y;

		if (xOverlap >= 0 && yOverlap >= 0) {
			bool vertically = m_enemyLastOverlap[enemy].x > 0;
			bool horizontally = m_enemyLastOverlap[enemy].y > 0;
			// came right
			if (horizontally && playerTransform.pos.x > enemyTransform.pos.x) {
				playerTransform.pos.x += xOverlap;
				xOverlap = 0;
			}
			// came left
			else if (horizontally && playerTransform.pos.x < enemyTransform.pos.x) {
				playerTransform.pos.x -= xOverlap;
				xOverlap = 0;
			}
			// came top
			else if (vertically && playerTransform.pos.y < enemyTransform.pos.y) {
				playerTransform.pos.y -= yOverlap;
				yOverlap = 0;
			}
			//came bottom
			else if (vertically && playerTransform.pos.y > enemyTransform.pos.y) {
				playerTransform.pos.y += yOverlap;
				yOverlap = 0;
			}
		}

		m_enemyLastOverlap[enemy].x = xOverlap;
		m_enemyLastOverlap[enemy].y = yOverlap;
	}
}

void Scene_Main::spawnEnemy(float x, float y) {
	auto enemy = m_entities.addEntity("enemy");

	enemy->addComponent<CTransform>(Vec2(x, y), Vec2(0.0f, 0.0f), 0.0f);
	enemy->addComponent<CBoundingBox>(Vec2(100.0f, 100.0f));
}

void Scene_Main::spawnPlayer() {
	auto entity = m_entities.addEntity("player");

	float mid_x = m_game->getWindow().getSize().x / 2.0f;
	float mid_y = m_game->getWindow().getSize().y / 2.0f;

	entity->addComponent<CTransform>(Vec2(mid_x, mid_y), Vec2(0.0f, 0.0f), 0.0f);
	entity->addComponent<CBoundingBox>(Vec2(80.0f, 80.0f));
	entity->addComponent<CInput>();
	m_player = entity;
}

void Scene_Main::sMovement() {

	auto& playerTransform = m_player->getComponent<CTransform>();

	playerTransform.velocity = { 0, 0 };
	int x = 0;
	int y = 0;

	if (m_player->getComponent<CInput>().up) {
		y = -5;
		playerTransform.velocity.y = y;
	}
	if (m_player->getComponent<CInput>().left) {
		x = -5;
		playerTransform.velocity.x = x;
	}
	if (m_player->getComponent<CInput>().down) {
		y = 5;
		playerTransform.velocity.y = y;
	}
	if (m_player->getComponent<CInput>().right) {
		x = 5;
		playerTransform.velocity.x = x;
	}

	if (x != 0 && y != 0) {
		// normalize diagonal speed
		playerTransform.velocity.x = x / 1.4;
		playerTransform.velocity.y = y / 1.4;
	}

	playerTransform.pos.x += playerTransform.velocity.x;
	playerTransform.pos.y += playerTransform.velocity.y;
}

void Scene_Main::sDoAction(const Action& action) {

	auto& playerInput = m_player->getComponent<CInput>();

	if (action.getName() == "PAUSE") {
		setPaused(!m_paused);
	}

	if (action.getName() == "UP") {
		if (action.getType() == "START") {
			playerInput.up = true;
			m_playerAnimation = "player_animation_up";
		}
		if (action.getType() == "END") {
			checkAnimationDirections(false, true, true, true);
			m_playerStandingTexture = "player_up";
			playerInput.up = false;
		}
	}

	if (action.getName() == "DOWN") {
		if (action.getType() == "START") {
			playerInput.down = true;
			m_playerAnimation = "player_animation";
		}
		if (action.getType() == "END") {
			checkAnimationDirections(true, false, true, true);
			m_playerStandingTexture = "player";
			playerInput.down = false;
		}
	}

	if (action.getName() == "LEFT") {
		if (action.getType() == "START") {
			playerInput.left = true;
			m_playerAnimation = "player_animation_left";
		}
		if (action.getType() == "END") {
			checkAnimationDirections(true, true, false, true);
			m_playerStandingTexture = "player_left";
			playerInput.left = false;
		}
	}

	if (action.getName() == "RIGHT") {
		if (action.getType() == "START") {
			playerInput.right = true;
			m_playerAnimation = "player_animation_right";
		}
		if (action.getType() == "END") {
			checkAnimationDirections(true, true, true, false);
			m_playerStandingTexture = "player_right";
			playerInput.right = false;
		}
	}
}

// if multiple direction were pressed, the animation would be stuck on the last released direction
void Scene_Main::checkAnimationDirections(bool up, bool down, bool left, bool right) {

	auto& playerInput = m_player->getComponent<CInput>();

	if (up && playerInput.up) {
		m_playerAnimation = "player_animation_up";
	}
	if (left && playerInput.left) {
		m_playerAnimation = "player_animation_left";
	}
	if (down && playerInput.down) {
		m_playerAnimation = "player_animation";
	}
	if (right && playerInput.right) {
		m_playerAnimation = "player_animation_right";
	}
}

void Scene_Main::onEnd() {
	m_game->quit();
}
