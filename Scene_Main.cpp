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
		
		if (e->getTag() != "player") {
			sf::RectangleShape rect;
			rect.setFillColor(sf::Color(255, 0, 0));
			rect.setOutlineColor(sf::Color(0, 0, 255));
			rect.setPosition(sf::Vector2f(
				e->cTransform->pos.x - e->cBoundingBox->halfSize.x,
				e->cTransform->pos.y - e->cBoundingBox->halfSize.y));
			rect.setSize(sf::Vector2f(e->cBoundingBox->size.x, e->cBoundingBox->size.y));
			m_game->getWindow().draw(rect);

			sf::RectangleShape point;
			point.setFillColor(sf::Color(0, 255, 0));
			point.setPosition(sf::Vector2f(e->cTransform->pos.x -1, e->cTransform->pos.y -1));
			point.setSize(sf::Vector2f(3, 3));
			m_game->getWindow().draw(point);
		}
		else {

			sf::RectangleShape player(sf::Vector2f(80, 80));
			if (m_player->cInput->up || m_player->cInput->down || m_player->cInput->left || m_player->cInput->right) {
				player.setTexture(m_game->getAssets().getAnimation(m_playerAnimation).getSprite().getTexture());
				player.setTextureRect(m_game->getAssets().getAnimation(m_playerAnimation).getSprite().getTextureRect());
				m_game->getAssets().getAnimation(m_playerAnimation).update();
			}
			else {
				player.setTexture(&m_game->getAssets().getTexture(m_playerStandingTexture));
			}
			player.setPosition(
				m_player->cTransform->pos.x - m_player->cBoundingBox->halfSize.x, 
				m_player->cTransform->pos.y - m_player->cBoundingBox->halfSize.y);
			m_game->getWindow().draw(player);
		}
		
	}
	m_game->getWindow().display();
}

void Scene_Main::sCollision() {

	for (auto enemy : m_entities.getEntities("enemy")) {

		Vec2 delta = Vec2(
			abs(m_player->cTransform->pos.x - enemy->cTransform->pos.x), 
			abs(m_player->cTransform->pos.y - enemy->cTransform->pos.y));
		float xOverlap = m_player->cBoundingBox->halfSize.x + enemy->cBoundingBox->halfSize.x - delta.x;
		float yOverlap = m_player->cBoundingBox->halfSize.y + enemy->cBoundingBox->halfSize.y - delta.y;

		if (xOverlap >= 0 && yOverlap >= 0) {
			bool vertically = m_enemyLastOverlap[enemy].x > 0;
			bool horizontally = m_enemyLastOverlap[enemy].y > 0;
			// came right
			if (horizontally && m_player->cTransform->pos.x > enemy->cTransform->pos.x) {
				m_player->cTransform->pos.x += xOverlap;
				xOverlap = 0;
			}
			// came left
			else if (horizontally && m_player->cTransform->pos.x < enemy->cTransform->pos.x) {
				m_player->cTransform->pos.x -= xOverlap;
				xOverlap = 0;
			}
			// came top
			else if (vertically && m_player->cTransform->pos.y < enemy->cTransform->pos.y) {
				m_player->cTransform->pos.y -= yOverlap;
				yOverlap = 0;
			}
			//came bottom
			else if (vertically && m_player->cTransform->pos.y > enemy->cTransform->pos.y) {
				m_player->cTransform->pos.y += yOverlap;
				yOverlap = 0;
			}
		}

		m_enemyLastOverlap[enemy].x = xOverlap;
		m_enemyLastOverlap[enemy].y = yOverlap;
	}
}

void Scene_Main::spawnEnemy(float x, float y) {
	auto enemy = m_entities.addEntity("enemy");

	enemy->cTransform = std::make_shared<CTransform>(Vec2(x, y), Vec2(0.0f, 0.0f), 0.0f);
	enemy->cBoundingBox = std::make_shared<CBoundingBox>(Vec2(100.0f, 100.0f));
}

void Scene_Main::spawnPlayer() {
	auto entity = m_entities.addEntity("player");

	float mid_x = m_game->getWindow().getSize().x / 2.0f;
	float mid_y = m_game->getWindow().getSize().y / 2.0f;

	entity->cTransform = std::make_shared<CTransform>(Vec2(mid_x, mid_y), Vec2(0.0f, 0.0f), 0.0f);
	entity->cBoundingBox = std::make_shared<CBoundingBox>(Vec2(80.0f, 80.0f));
	entity->cInput = std::make_shared<CInput>();
	m_player = entity;
}

void Scene_Main::sMovement() {

	m_player->cTransform->velocity = { 0, 0 };
	int x = 0;
	int y = 0;

	if (m_player->cInput->up) {
		y = -5;
		m_player->cTransform->velocity.y = y;
	}
	if (m_player->cInput->left) {
		x = -5;
		m_player->cTransform->velocity.x = x;
	}
	if (m_player->cInput->down) {
		y = 5;
		m_player->cTransform->velocity.y = y;
	}
	if (m_player->cInput->right) {
		x = 5;
		m_player->cTransform->velocity.x = x;
	}

	if (x != 0 && y != 0) {
		// normalize diagonal speed
		m_player->cTransform->velocity.x = x / 1.4;
		m_player->cTransform->velocity.y = y / 1.4;
	}

	m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
	m_player->cTransform->pos.y += m_player->cTransform->velocity.y;
}

void Scene_Main::sDoAction(const Action& action) {
	if (action.getName() == "PAUSE") {
		setPaused(!m_paused);
	}

	if (action.getName() == "UP") {
		if (action.getType() == "START") {
			m_player->cInput->up = true;
			m_playerAnimation = "player_animation_up";
		}
		if (action.getType() == "END") {
			checkAnimationDirections(false, true, true, true);
			m_playerStandingTexture = "player_up";
			m_player->cInput->up = false;
		}
	}

	if (action.getName() == "DOWN") {
		if (action.getType() == "START") {
			m_player->cInput->down = true;
			m_playerAnimation = "player_animation";
		}
		if (action.getType() == "END") {
			checkAnimationDirections(true, false, true, true);
			m_playerStandingTexture = "player";
			m_player->cInput->down = false;
		}
	}

	if (action.getName() == "LEFT") {
		if (action.getType() == "START") {
			m_player->cInput->left = true;
			m_playerAnimation = "player_animation_left";
		}
		if (action.getType() == "END") {
			checkAnimationDirections(true, true, false, true);
			m_playerStandingTexture = "player_left";
			m_player->cInput->left = false;
		}
	}

	if (action.getName() == "RIGHT") {
		if (action.getType() == "START") {
			m_player->cInput->right = true;
			m_playerAnimation = "player_animation_right";
		}
		if (action.getType() == "END") {
			checkAnimationDirections(true, true, true, false);
			m_playerStandingTexture = "player_right";
			m_player->cInput->right = false;
		}
	}
}

// if multiple direction were pressed, the animation would be stuck on the last released direction
void Scene_Main::checkAnimationDirections(bool up, bool down, bool left, bool right) {
	if (up && m_player->cInput->up) {
		m_playerAnimation = "player_animation_up";
	}
	if (left && m_player->cInput->left) {
		m_playerAnimation = "player_animation_left";
	}
	if (down && m_player->cInput->down) {
		m_playerAnimation = "player_animation";
	}
	if (right && m_player->cInput->right) {
		m_playerAnimation = "player_animation_right";
	}
}

void Scene_Main::onEnd() {
	m_game->quit();
}
