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
	m_inventorySys = std::make_shared<InventorySystem>();

	m_inventorySys->addItem("Health Potion", 3);
	m_inventorySys->addItem("Mana Potion", 3);

	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::P, "DEBUG");
	registerAction(sf::Keyboard::Enter, "ACCEPT");
	registerAction(sf::Keyboard::Escape, "PAUSE");
	registerAction(sf::Keyboard::O, "FIGHT");

	loadMap("assets/main_map.json");

	spawnPlayer();
	
	m_camera = sf::View(
		sf::Vector2f(m_game->getWindow().getSize().x / 2, m_game->getWindow().getSize().y / 2), 
		sf::Vector2f(m_game->getWindow().getSize().x, m_game->getWindow().getSize().y));
	m_game->getWindow().setView(m_camera);

	m_game->setDebugMode(true);

	m_shader.loadFromFile(m_game->m_shader, sf::Shader::Fragment);
}

void Scene_Main::loadMap(const std::string& path) {
	m_game->loadGameMap(path, m_entities);
}

void Scene_Main::update() {
	m_entities.update();
	updateZIndexes();
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
	m_player->getComponent<CState>().setCustomState("ready");
	m_fight = false;
	m_game->changeScene("fight", std::make_shared<Scene_Fight>(m_game, m_player, m_inventorySys));
}

void Scene_Main::sRender() {
	m_game->getWindow().clear();

	// background
	sf::RectangleShape background(sf::Vector2f(1500, 1500));
	background.setFillColor(sf::Color(32, 32, 32));
	background.setPosition(-200, -200);
	m_game->getWindow().draw(background);

	for (auto& e : m_entities.getEntities()) {

		if (e->getTag() == "player") {
			renderPlayer(e);
		}
		else {
			renderEntity(e);
		}

		if (m_game->isDebugMode()) {
			renderBoundingBox(e);
		}
	}

	if (!m_textBoxSys.getText().empty()) {
		m_textBoxSys.render(m_currentFrame);
	}
	if (m_sceneChanged) {
		auto windowSize = m_game->getWindow().getSize();
		renderTransitionAnimation(true);
		if (m_transitionOpacity >= 255) {
			m_fight = true;
		}
	}

	m_game->getWindow().display();
}

void Scene_Main::renderPlayer(const std::shared_ptr<Entity>& e) {
	auto& playerInput = m_player->getComponent<CInput>();
	auto& playerGraphics = m_player->getComponent<CGraphics>();
	
	sf::RectangleShape playerRect(sf::Vector2f(80, 80));
	if (playerInput.up || playerInput.down || playerInput.left || playerInput.right) {

		playerRect.setTexture(playerGraphics.animation.getSprite().getTexture());
		playerRect.setTextureRect(playerGraphics.animation.getSprite().getTextureRect());

		playerGraphics.animation.update();
	}
	else {
		playerGraphics.animation.reset();
		playerRect.setTexture(&m_game->getAssets().getTexture(m_player->getComponent<CGraphics>().texture));
	}

	Vec2 pos = e->getComponent<CTransform>().getTopLeftPos();
	playerRect.setPosition(pos.x, pos.y);
	// Maybe draw the sprite without the playerRect
	float time = m_clock.getElapsedTime().asSeconds();
	m_shader.setUniform("u_time", time);
	m_game->getWindow().draw(playerRect, &m_shader);
}

void Scene_Main::renderEntity(const std::shared_ptr<Entity>& e) {
	auto& transform = e->getComponent<CTransform>();
	Vec2 pos = e->getComponent<CTransform>().getTopLeftPos();

	// Draw tile maps
	if (e->hasComponent<CGraphics>() && e->getComponent<CGraphics>().textureMap) {
		auto& graphics = e->getComponent<CGraphics>();
		sf::Sprite sprite(m_game->getAssets().getTexture(graphics.texture));
		sprite.setScale(sf::Vector2f(transform.scale.x, transform.scale.y));
		for (auto& tile : graphics.tiles) {
			sprite.setTextureRect(sf::IntRect(tile.texturePos.x, tile.texturePos.y, tile.textureSize.x, tile.textureSize.y));
			float xPos = graphics.textureMapStartingPos.x + (transform.scale.x * tile.textureSize.x * tile.posIndex.x);
			float yPos = graphics.textureMapStartingPos.y + (transform.scale.y * tile.textureSize.y * tile.posIndex.y);
			sprite.setPosition(xPos, yPos);

			m_game->getWindow().draw(sprite);
		}
		return;
	}

	// Draw repeated textures
	if (e->hasComponent<CGraphics>() && e->getComponent<CGraphics>().repeated) {
		sf::Sprite sprite(m_game->getAssets().getTexture(e->getComponent<CGraphics>().texture));
		sprite.setTextureRect(sf::IntRect(0, 0, transform.size.x, transform.size.y));
		sprite.setPosition(pos.x, pos.y);
		sprite.setScale(sf::Vector2f(transform.scale.x, transform.scale.y));

		m_game->getWindow().draw(sprite);
		return;
	}

	sf::RectangleShape rect(sf::Vector2f(transform.size.x, transform.size.y));
	rect.setPosition(pos.x, pos.y);
	rect.setScale(sf::Vector2f(transform.scale.x, transform.scale.y));

	if (e->hasComponent<CGraphics>()) {
		rect.setTexture(&m_game->getAssets().getTexture(e->getComponent<CGraphics>().texture));
	}

	m_game->getWindow().draw(rect);
}

void Scene_Main::renderBoundingBox(const std::shared_ptr<Entity>& e) {
	if (!e->hasComponent<CBoundingBox>()) return;

	auto& entityBoundingBox = e->getComponent<CBoundingBox>();
	auto& entityTransform = e->getComponent<CTransform>();

	sf::RectangleShape boundingBox(sf::Vector2f(entityBoundingBox.size.x, entityBoundingBox.size.y));
	boundingBox.setFillColor(sf::Color(255, 0, 0, 125));
	auto& pos = entityBoundingBox.getTopLeftPos(entityTransform.getPos());
	boundingBox.setPosition(pos.x, pos.y);
	m_game->getWindow().draw(boundingBox);

	sf::RectangleShape point(sf::Vector2f(3, 3));
	point.setFillColor(sf::Color(0, 255, 0));
	point.setPosition(sf::Vector2f(entityTransform.getPos().x - 1, entityTransform.getPos().y - 1));
	m_game->getWindow().draw(point);
}

void Scene_Main::renderTransitionAnimation(bool fadeOut) {
	sf::Vector2u WINDOW_SIZE = m_game->getWindow().getSize();
	auto& VIEW_POSITION = Physics::getViewPosition(m_game->getWindow().getView(),Vec2(WINDOW_SIZE.x, WINDOW_SIZE.y));

	sf::RectangleShape transition(sf::Vector2f(WINDOW_SIZE.x, WINDOW_SIZE.y));
	transition.setPosition(VIEW_POSITION.x, VIEW_POSITION.y);
	transition.setFillColor(sf::Color(0, 0, 0, m_transitionOpacity));
	m_game->getWindow().draw(transition);

	if (fadeOut) {
		m_transitionOpacity = m_transitionOpacity >= 255 ? 255 : m_transitionOpacity + 8;
	}
	else {
		m_transitionOpacity = m_transitionOpacity <= 0 ? 0 : m_transitionOpacity - 8;
	}
}

void Scene_Main::sCollision() {
	auto& playerBoundingBox = m_player->getComponent<CBoundingBox>();
	auto& playerTransform = m_player->getComponent<CTransform>();
	Vec2 playerBBPos = playerBoundingBox.getPos(playerTransform.getPos());

	for (auto& e : m_entities.getEntities()) {

		if (e->getTag() == "player") {
			continue;
		}

		if (e->getTag() == "door") {
			auto& door = m_entities.getEntities("door")[0];
			Vec2 doorCollision = Physics::getOverlap(m_player, door);
			if (doorCollision.x > 0 && doorCollision.y > 0) {
				m_player->getComponent<CTransform>().setPosition(500.0f, 500.0f);
				cameraToPlayer();
			}
			continue;
		}

		if (!e->hasComponent<CBoundingBox>() || !e->hasComponent<CTransform>()) {
			continue;
		}

		Vec2 overlap = Physics::getOverlap(m_player, e);
		Vec2 lastOverlap = Physics::getPreviousOverlap(m_player, e);

		if (overlap.x > 0 && overlap.y > 0) {

			auto& entityBoundingBox = e->getComponent<CBoundingBox>();
			Vec2 entityBBPos = entityBoundingBox.getPos(e->getComponent<CTransform>().getPos());

			bool vertically = lastOverlap.x > 0;
			bool horizontally = lastOverlap.y > 0;
			// came right
			if (horizontally && playerBBPos.x > entityBBPos.x) {
				playerTransform.setX(playerTransform.getPos().x + overlap.x);
			}
			// came left
			else if (horizontally && playerBBPos.x < entityBBPos.x) {
				playerTransform.setX(playerTransform.getPos().x - overlap.x);
			}
			// came top
			else if (vertically && playerBBPos.y < entityBBPos.y) {
				playerTransform.setY(playerTransform.getPos().y - overlap.y);
			}
			//came bottom
			else if (vertically && playerBBPos.y > entityBBPos.y) {
				playerTransform.setY(playerTransform.getPos().y + overlap.y);
			}
		}
	}
}

void Scene_Main::sMovement() {
	auto& playerTransform = m_player->getComponent<CTransform>();

	playerTransform.velocity = { 0, 0 };
	int x = 0;
	int y = 0;

	if (m_player->getComponent<CState>().getCustomState() == "freeze") {
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
	auto& playerState = m_player->getComponent<CState>();

	if (action.getName() == "ACCEPT" && action.getType() == Action::START) {

		for (auto& e : m_entities.getEntities()) {
			float dist = m_player->getComponent<CTransform>().getPos().dist(e->getComponent<CTransform>().getPos());
			if ((e->getTag() == "sign1" || e->getTag() == "sign2") && dist <= 40) {

				if (playerState.getCustomState() == "ready") {
					playerState.setCustomState("freeze");
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
					playerState.setCustomState("ready");
				}
			}
		}
	}

	if (action.getName() == "DEBUG" && action.getType() == Action::START) {
		m_game->setDebugMode(!m_game->isDebugMode());
	}

	auto& playerInput = m_player->getComponent<CInput>();

	if (playerState.getCustomState() == "freeze") {
		if (playerInput.up) {
			playerInput.up = false;
			m_player->getComponent<CGraphics>().texture = "player_up";
		}
		else if (playerInput.down) {
			playerInput.down = false;
			m_player->getComponent<CGraphics>().texture = "player";
		}
		else if (playerInput.left) {
			playerInput.left = false;
			m_player->getComponent<CGraphics>().texture = "player_left";
		}
		else if (playerInput.right) {
			playerInput.right = false;
			m_player->getComponent<CGraphics>().texture = "player_right";
		}
		return;
	}

	handlePlayerMovement(action, playerInput);

	if (action.getName() == "PAUSE" && action.getType() == Action::START) {
		m_game->changeScene("pause", std::make_shared<Scene_Pause>(m_game, m_inventorySys));
	}

	if (action.getName() == "FIGHT" && action.getType() == Action::START) {
		m_sceneChanged = true;
		playerState.setCustomState("freeze");
	}
}

void Scene_Main::handlePlayerMovement(const Action& action, CInput& playerInput) {
	if (action.getName() == "UP") {
		if (action.getType() == Action::START) {
			playerInput.up = true;
			changeAnimation(m_player, "player_animation_up");
		}
		if (action.getType() == Action::END) {
			correctAnimationDirections(false, true, true, true);
			m_player->getComponent<CGraphics>().texture = "player_up";
			playerInput.up = false;
		}
	}

	if (action.getName() == "DOWN") {
		if (action.getType() == Action::START) {
			playerInput.down = true;
			changeAnimation(m_player, "player_animation");
		}
		if (action.getType() == Action::END) {
			correctAnimationDirections(true, false, true, true);
			m_player->getComponent<CGraphics>().texture = "player";
			playerInput.down = false;
		}
	}

	if (action.getName() == "LEFT") {
		if (action.getType() == Action::START) {
			playerInput.left = true;
			changeAnimation(m_player, "player_animation_left");
		}
		if (action.getType() == Action::END) {
			correctAnimationDirections(true, true, false, true);
			m_player->getComponent<CGraphics>().texture = "player_left";
			playerInput.left = false;
		}
	}

	if (action.getName() == "RIGHT") {
		if (action.getType() == Action::START) {
			playerInput.right = true;
			changeAnimation(m_player, "player_animation_right");
		}
		if (action.getType() == Action::END) {
			correctAnimationDirections(true, true, true, false);
			m_player->getComponent<CGraphics>().texture = "player_right";
			playerInput.right = false;
		}
	}
}

// AUGENKREBS
// if multiple directions were pressed, the animation would be stuck on the last released direction
void Scene_Main::correctAnimationDirections(bool up, bool down, bool left, bool right) {
	auto& playerInput = m_player->getComponent<CInput>();

	if (up && playerInput.up) {
		changeAnimation(m_player, "player_animation_up");
	}
	if (left && playerInput.left) {
		changeAnimation(m_player, "player_animation_left");
	}
	if (down && playerInput.down) {
		changeAnimation(m_player, "player_animation");
	}
	if (right && playerInput.right) {
		changeAnimation(m_player, "player_animation_right");
	}
}

void Scene_Main::changeAnimation(const std::shared_ptr<Entity>& entity, const std::string& animationName) {
	if (!entity->hasComponent<CGraphics>()) {
		return;
	}
	if (entity->getComponent<CGraphics>().animation.getName() != animationName) {
		entity->addComponent<CGraphics>(m_game->getAssets().getAnimation(animationName));
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

	auto& transform = entity->addComponent<CTransform>(Vec2(mid_x, mid_y), Vec2(0.0f, 0.0f), 0.0f, Vec2(80.0, 80.0));
	transform.zIndex = 1000; // Just a random high number to prevent entities to be drawn over it
	entity->addComponent<CBoundingBox>(Vec2(80.0f, 40.0f), Vec2(0, 20.0f));
	entity->addComponent<CInput>();
	entity->addComponent<CGraphics>("player");
	entity->addComponent<CState>("ready");

	auto& stats = entity->addComponent<CStats>(50, 50);
	stats.addAttack("Fire", 11, CStats::FIRE);
	stats.addAttack("Ice", 8, CStats::ICE);
	stats.addAttack("Poison", 3, CStats::POISON);
	stats.addAttack("Lightning", 10, CStats::LIGHTNING);

	m_player = entity;
}

// Check if the player is above or below the other entity to make it appear as if the player goes behind the entity
void Scene_Main::updateZIndexes() {
	auto& playerTransform = m_player->getComponent<CTransform>();
	Vec2 playerBBPos = m_player->getComponent<CBoundingBox>().getPos(playerTransform.getPos());

	for (auto& e : m_entities.getEntities()) {
		if ((!e->hasComponent<CBoundingBox>() || !e->hasComponent<CTransform>())
			|| (e->hasComponent<CGraphics>() && e->getComponent<CGraphics>().background)
			|| (e->getTag() == "player")) {
			continue;
		}

		auto& entityTransform = e->getComponent<CTransform>();
		Vec2 entityBBPos = e->getComponent<CBoundingBox>().getPos(entityTransform.getPos());

		if (playerBBPos.y < entityBBPos.y) {
			entityTransform.zIndex = playerTransform.zIndex + 1;
		}
		else {
			entityTransform.zIndex = playerTransform.zIndex - 1;
		}
	}
}

void Scene_Main::onEnd() {
	m_game->quit();
}
