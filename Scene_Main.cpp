#include "Scene_Main.h"
#include "GameEngine.h"
#include "Physics.h"
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
	m_camera = sf::View(
		sf::Vector2f(m_game->getWindow().getSize().x / 2, m_game->getWindow().getSize().y / 2), 
		sf::Vector2f(m_game->getWindow().getSize().x, m_game->getWindow().getSize().y));
	m_game->getWindow().setView(m_camera);
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

	m_camera.setCenter(sf::Vector2f(m_player->getComponent<CTransform>().getPos().x, m_player->getComponent<CTransform>().getPos().y));
	m_game->getWindow().setView(m_camera);

	m_currentFrame++;
}

void Scene_Main::sRender() {
	m_game->getWindow().clear();
	for (auto e : m_entities.getEntities()) {

		auto& entityTransform = e->getComponent<CTransform>();
		
		if (e->getTag() == "player") {
		
			auto& playerInput = m_player->getComponent<CInput>();

			sf::RectangleShape playerRect(sf::Vector2f(80, 80));
			if (playerInput.up || playerInput.down || playerInput.left || playerInput.right) {

				auto& animationComponent = m_player->getComponent<CAnimation>();
				playerRect.setTexture(animationComponent.animation.getSprite().getTexture());
				playerRect.setTextureRect(animationComponent.animation.getSprite().getTextureRect());
				
				if (animationComponent.repeat || (!animationComponent.repeat && !animationComponent.animation.hasEnded())) {
					animationComponent.animation.update();
				}
			}
			else {
				playerRect.setTexture(&m_game->getAssets().getTexture(m_playerStandingTexture));
			}
			playerRect.setPosition(
				entityTransform.getPos().x - (playerRect.getSize().x / 2),
				entityTransform.getPos().y - (playerRect.getSize().y / 2));
			// Maybe draw the sprite without the playerRect
			m_game->getWindow().draw(playerRect);

		}
		renderBoundingBox(e);
	}
	m_game->getWindow().display();
}

void Scene_Main::renderBoundingBox(const std::shared_ptr<Entity>& entity) {
	auto& entityBoundingBox = entity->getComponent<CBoundingBox>();
	auto& entityTransform = entity->getComponent<CTransform>();

	sf::RectangleShape boundingBox;
		boundingBox.setFillColor(sf::Color(255, 0, 0, 125));
		boundingBox.setOutlineColor(sf::Color(0, 0, 255));
		boundingBox.setSize(sf::Vector2f(entityBoundingBox.size.x, entityBoundingBox.size.y));
		boundingBox.setPosition(
			entityTransform.getPos().x - (boundingBox.getSize().x / 2) + entityBoundingBox.relativePosition.x,
			entityTransform.getPos().y - (boundingBox.getSize().y / 2) + entityBoundingBox.relativePosition.y);
	m_game->getWindow().draw(boundingBox);

	sf::RectangleShape point;
	point.setFillColor(sf::Color(0, 255, 0));
	point.setPosition(sf::Vector2f(entityTransform.getPos().x - 1, entityTransform.getPos().y - 1));
	point.setSize(sf::Vector2f(3, 3));
	m_game->getWindow().draw(point);
}


void Scene_Main::sCollision() {

	auto& playerTransform = m_player->getComponent<CTransform>();

	for (auto enemy : m_entities.getEntities("enemy")) {

		auto& enemyTransform = enemy->getComponent<CTransform>();

		Vec2 overlap = Physics::getOverlap(m_player, enemy);
		Vec2 lastOverlap = Physics::getPreviousOverlap(m_player, enemy);

		if (overlap.x >= 0 && overlap.y >= 0) {
			bool vertically = lastOverlap.x > 0;
			bool horizontally = lastOverlap.y > 0;
			// came right
			if (horizontally && playerTransform.getPos().x > enemyTransform.getPos().x) {
				playerTransform.setX(playerTransform.getPos().x + overlap.x);
			}
			// came left
			else if (horizontally && playerTransform.getPos().x < enemyTransform.getPos().x) {
				playerTransform.setX(playerTransform.getPos().x - overlap.x);
			}
			// came top
			else if (vertically && playerTransform.getPos().y < enemyTransform.getPos().y) {
				playerTransform.setY(playerTransform.getPos().y - overlap.y);
			}
			//came bottom
			else if (vertically && playerTransform.getPos().y > enemyTransform.getPos().y) {
				playerTransform.setY(playerTransform.getPos().y + overlap.y);
			}
		}
	}
}

void Scene_Main::spawnEnemy(float x, float y) {
	auto enemy = m_entities.addEntity("enemy");

	enemy->addComponent<CTransform>(Vec2(x, y), Vec2(0.0f, 0.0f), 0.0f);
	enemy->addComponent<CBoundingBox>(Vec2(100.0f, 100.0f), Vec2(0, 0));
}

void Scene_Main::spawnPlayer() {
	auto entity = m_entities.addEntity("player");

	float mid_x = m_game->getWindow().getSize().x / 2.0f;
	float mid_y = m_game->getWindow().getSize().y / 2.0f;

	entity->addComponent<CTransform>(Vec2(mid_x, mid_y), Vec2(0.0f, 0.0f), 0.0f);
	entity->addComponent<CBoundingBox>(Vec2(80.0f, 40.0f), Vec2(0, 20.0f));
	entity->addComponent<CInput>();
	entity->addComponent<CAnimation>();
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

	playerTransform.setPosition(
		playerTransform.getPos().x + playerTransform.velocity.x,
		playerTransform.getPos().y + playerTransform.velocity.y);
}

void Scene_Main::sDoAction(const Action& action) {

	auto& playerInput = m_player->getComponent<CInput>();

	if (action.getName() == "PAUSE") {
		setPaused(!m_paused);
	}

	if (action.getName() == "UP") {
		if (action.getType() == "START") {
			playerInput.up = true;
			changeAnimation(m_player, "player_animation_up", true);
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
			changeAnimation(m_player, "player_animation", true);
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
			changeAnimation(m_player, "player_animation_left", true);
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
			changeAnimation(m_player, "player_animation_right", true);
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
		changeAnimation(m_player, "player_animation_up", true);
	}
	if (left && playerInput.left) {
		changeAnimation(m_player, "player_animation_left", true);
	}
	if (down && playerInput.down) {
		changeAnimation(m_player, "player_animation", true);
	}
	if (right && playerInput.right) {
		changeAnimation(m_player, "player_animation_right", true);
	}
}

void Scene_Main::changeAnimation(const std::shared_ptr<Entity>& entity, const std::string& animationName, bool repeat) {
	if (!entity->hasComponent<CAnimation>()) {
		return;
	}
	if (entity->getComponent<CAnimation>().animation.getName() != animationName) {
		entity->addComponent<CAnimation>(m_game->getAssets().getAnimation(animationName), repeat);
	}
}

void Scene_Main::onEnd() {
	m_game->quit();
}
