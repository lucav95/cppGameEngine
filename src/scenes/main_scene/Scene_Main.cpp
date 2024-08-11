#include "Scene_Main.h"
#include "../Scene_Pause.h"
#include "../Scene_Fight.h"
#include "../../engine/GameEngine.h"
#include "../../engine/Physics.h"
#include <iostream>
#include <sstream>

Scene_Main::Scene_Main(GameEngine* game)
	: Scene(game) {
	init();
}

void Scene_Main::init() {
	m_textBoxSys = TextBoxSystem(m_game);

	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::P, "DEBUG");
	registerAction(sf::Keyboard::Enter, "ACCEPT");
	registerAction(sf::Keyboard::Escape, "PAUSE");
	registerAction(sf::Keyboard::O, "FIGHT");

	spawnEntity(Vec2(120, 300), Vec2(100, 100), "enemy");
	spawnEntity(Vec2(300, 300), Vec2(100, 100), "enemy");
	spawnEntity(Vec2(500, 100), Vec2(90, 130), "door");
	spawnEntity(Vec2(620, 150), Vec2(60, 60), "sign1");
	spawnEntity(Vec2(770, 150), Vec2(60, 60), "sign2");
	
	spawnPlayer();
	m_camera = sf::View(
		sf::Vector2f(m_game->getWindow().getSize().x / 2, m_game->getWindow().getSize().y / 2), 
		sf::Vector2f(m_game->getWindow().getSize().x, m_game->getWindow().getSize().y));
	m_game->getWindow().setView(m_camera);

	m_game->setDebugMode(true);
}

void Scene_Main::update() {
	m_entities.update();
	sMovement();
	sCollision();
	sRender();
	cameraToPlayer();

	if (m_fight) {
		fight();
	}

	m_currentFrame++;
}

void Scene_Main::fight() {
	m_transitionOpacity = 0;
	m_sceneChanged = false;
	m_freezePlayer = false;
	m_fight = false;
	m_game->changeScene("fight", std::make_shared<Scene_Fight>(m_game, m_player));
}

void Scene_Main::sRender() {
	m_game->getWindow().clear();

	// background
	sf::RectangleShape background(sf::Vector2f(1500, 1500));
	background.setFillColor(sf::Color(32, 32, 32));
	background.setPosition(-200, -200);
	m_game->getWindow().draw(background);

	for (auto e : m_entities.getEntities()) {
		
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

			auto& entityTransform = e->getComponent<CTransform>();
			playerRect.setPosition(
				entityTransform.getPos().x - (playerRect.getSize().x / 2),
				entityTransform.getPos().y - (playerRect.getSize().y / 2));
			// Maybe draw the sprite without the playerRect
			m_game->getWindow().draw(playerRect);

		}

		if (e->getTag() == "door") {
			sf::RectangleShape door(sf::Vector2f(90.0f, 130.0f));
			door.setPosition(
				e->getComponent<CTransform>().getPos().x - (door.getSize().x / 2),
				e->getComponent<CTransform>().getPos().y - (door.getSize().y / 2));
			door.setTexture(&m_game->getAssets().getTexture("door"));
			m_game->getWindow().draw(door);
		}

		if (e->getTag() == "sign1" || e->getTag() == "sign2") {
			sf::RectangleShape sign(sf::Vector2f(60, 60));
			sign.setPosition(
				e->getComponent<CTransform>().getPos().x - (sign.getSize().x / 2),
				e->getComponent<CTransform>().getPos().y - (sign.getSize().y / 2));
			sign.setTexture(&m_game->getAssets().getTexture("sign"));
			m_game->getWindow().draw(sign);
		}
		renderBoundingBox(e);
	}
	if (!m_textBoxSys.getText().empty()) {
		m_textBoxSys.render(m_currentFrame);
	}
	if (m_sceneChanged) {
		auto windowSize = m_game->getWindow().getSize();
		renderTransition(Physics::getViewPosition(
			m_game->getWindow().getView(), 
			Vec2(windowSize.x, windowSize.y)));
	}

	m_game->getWindow().display();
}

void Scene_Main::renderBoundingBox(const std::shared_ptr<Entity>& entity) {
	if (!m_game->isDebugMode()) return;

	auto& entityBoundingBox = entity->getComponent<CBoundingBox>();
	auto& entityTransform = entity->getComponent<CTransform>();

	sf::RectangleShape boundingBox;
		boundingBox.setFillColor(sf::Color(255, 0, 0, 125));
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

void Scene_Main::renderTransition(const Vec2& viewPosition) {
	if (m_transitionOpacity >= 255) {
		m_fight = true;
	}
	
	sf::Vector2u WINDOW_SIZE = m_game->getWindow().getSize();

	sf::RectangleShape transition;
	transition.setPosition(viewPosition.x, viewPosition.y);
	transition.setSize(sf::Vector2f(WINDOW_SIZE.x, WINDOW_SIZE.y));
	transition.setFillColor(sf::Color(0, 0, 0, m_transitionOpacity));
	m_game->getWindow().draw(transition);

	m_transitionOpacity += 8;
}

void Scene_Main::sCollision() {

	auto& playerTransform = m_player->getComponent<CTransform>();

	for (auto e : m_entities.getEntities()) {

		if (e->getTag() == "enemy" || e->getTag() == "sign1" || e->getTag() == "sign2") {

			auto& enemyTransform = e->getComponent<CTransform>();

			Vec2 overlap = Physics::getOverlap(m_player, e);
			Vec2 lastOverlap = Physics::getPreviousOverlap(m_player, e);

			if (overlap.x > 0 && overlap.y > 0) {

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

	auto& door = m_entities.getEntities("door")[0];
	Vec2 doorCollision = Physics::getOverlap(m_player, door);
	if (doorCollision.x > 0 && doorCollision.y > 0) {
		m_player->getComponent<CTransform>().setPosition(500.0f, 500.0f);
		cameraToPlayer();
	}

}

void Scene_Main::sMovement() {

	auto& playerTransform = m_player->getComponent<CTransform>();

	playerTransform.velocity = { 0, 0 };
	int x = 0;
	int y = 0;

	if (m_freezePlayer) {
		return;
	}

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

	if (action.getName() == "ACCEPT" && action.getType() == "START") {

		for (auto e : m_entities.getEntities()) {
			float dist = m_player->getComponent<CTransform>().getPos().dist(e->getComponent<CTransform>().getPos());
			if ((e->getTag() == "sign1" || e->getTag() == "sign2") && dist <= 40) {

				if (!m_freezePlayer) {
					m_freezePlayer = true;
				}

				if (m_textBoxSys.getText().empty()) {
					m_textBoxSys.setText(m_game->getDialog(e->getTag()));
					m_textBoxSys.setCurrentBox(0);
				}
				else if (m_textBoxSys.getCurrentBox() < m_textBoxSys.getBoxCount() - 1) {
					m_textBoxSys.setCurrentBox(m_textBoxSys.getCurrentBox() + 1);
				}
				else {
					m_textBoxSys.setCurrentBox(0);
					m_textBoxSys.setText("");
					m_freezePlayer = false;
				}
			}
		}
	}

	if (action.getName() == "DEBUG" && action.getType() == "START") {
		std::cout << m_player->getComponent<CStats>().hp << "\n";
		m_game->setDebugMode(!m_game->isDebugMode());
	}

	auto& playerInput = m_player->getComponent<CInput>();

	if (m_freezePlayer) {
		if (playerInput.up) {
			playerInput.up = false;
			m_playerStandingTexture = "player_up";
		}
		else if (playerInput.down) {
			playerInput.down = false;
			m_playerStandingTexture = "player";
		}
		else if (playerInput.left) {
			playerInput.left = false;
			m_playerStandingTexture = "player_left";
		}
		else if (playerInput.right) {
			playerInput.right = false;
			m_playerStandingTexture = "player_right";
		}
		return;
	}

	handlePlayerMovement(action, playerInput);

	if (action.getName() == "PAUSE" && action.getType() == "START") {
		m_game->changeScene("pause", std::make_shared<Scene_Pause>(m_game));
	}

	if (action.getName() == "FIGHT" && action.getType() == "START") {
		m_sceneChanged = true;
		m_freezePlayer = true;
	}
}

void Scene_Main::handlePlayerMovement(const Action& action, CInput& playerInput) {
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

void Scene_Main::cameraToPlayer() {
	m_camera.setCenter(sf::Vector2f(m_player->getComponent<CTransform>().getPos().x, m_player->getComponent<CTransform>().getPos().y));
	m_game->getWindow().setView(m_camera);
}

void Scene_Main::spawnPlayer() {
	auto entity = m_entities.addEntity("player");

	float mid_x = m_game->getWindow().getSize().x / 2.0f;
	float mid_y = m_game->getWindow().getSize().y / 2.0f;

	entity->addComponent<CTransform>(Vec2(mid_x, mid_y), Vec2(0.0f, 0.0f), 0.0f);
	entity->addComponent<CBoundingBox>(Vec2(80.0f, 40.0f), Vec2(0, 20.0f));
	entity->addComponent<CInput>();
	entity->addComponent<CAnimation>();
	entity->addComponent<CStats>(50, 50);
	m_player = entity;
}

void Scene_Main::spawnEntity(const Vec2& pos, const Vec2& boundingBox, const std::string& name) {
	auto e = m_entities.addEntity(name);
	e->addComponent<CTransform>(pos, Vec2(0, 0), 0);
	e->addComponent<CBoundingBox>(boundingBox);
}

void Scene_Main::onEnd() {
	m_game->quit();
}
