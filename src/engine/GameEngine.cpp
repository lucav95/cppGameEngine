#include "GameEngine.h"
#include "../scenes/Scene_Menu.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <json.hpp>

GameEngine::GameEngine(
				const std::string& configPath, 
				const std::string& dialogPath) {
	init(configPath, dialogPath);
}

void GameEngine::init(
				const std::string& configPath, 
				const std::string& dialogPath) {
	loadAssets(configPath);
	loadDialog(dialogPath);

	m_window.create(sf::VideoMode(1280, 720), "Game", sf::Style::Close);
	m_window.setFramerateLimit(60);
	
	changeScene("menu", std::make_shared<Scene_Menu>(this));
}

void GameEngine::loadAssets(const std::string& configPath) {
	std::ifstream fin(configPath);
	nlohmann::json json = nlohmann::json::parse(fin);
	fin.close();

	for (auto& j : json) {
		if (j["type"] == "TEX") {
			m_assets.addTexture(j["name"], j["path"], j["repeated"].get<bool>());
		}
		if (j["type"] == "ANI") {
			m_assets.addTexture(j["name"], j["path"], false);
			Animation animation = Animation(
				j["name"], 
				m_assets.getTexture(j["name"]), 
				j["frameCount"].get<int>(), 
				j["speed"].get<int>());
			m_assets.addAnimation(j["name"], animation);
		}
		if (j["type"] == "FON") {
			m_assets.addFont(j["name"], j["path"]);
		}
		if (j["type"] == "SHA") {
			m_shader = j["path"].get<std::string>();
		}
	}
}

void GameEngine::loadDialog(const std::string& dialogPath) {
	std::ifstream fin(dialogPath);

	std::string line;
	while (std::getline(fin, line)) {

		std::stringstream stream(line);
		std::vector<std::string> row;
		std::string cell;
		while (std::getline(stream, cell, ';')) {
			row.push_back(cell);
		}
		m_dialogMap[row.at(0)] = row.at(1);
	}
	fin.close();
}

void GameEngine::loadGameMap(const std::string& gameMapPath, EntityManager& entities) {
	std::fstream fin(gameMapPath);
	nlohmann::json json = nlohmann::json::parse(fin);
	fin.close();

	for (auto& j : json) {
		if (j["type"] == "RECT") {

			auto e = entities.addEntity(j["name"]);

			if (j["texture"] != "none") {
				auto& graphics = e->addComponent<CGraphics>(j["texture"]);
				graphics.repeated = m_assets.getTexture(j["texture"]).isRepeated();
			}
			auto& transform = e->addComponent<CTransform>(
				Vec2(j["worldPos"]["x"].get<float>(), j["worldPos"]["y"].get<float>()),
				Vec2(0.0, 0.0),
				0,
				Vec2(j["size"]["w"].get<float>(), j["size"]["h"].get<float>()));
			transform.setScale(j["scale"]["w"].get<float>(), j["scale"]["h"].get<float>());
			if (j["boundingBox"]["w"].get<float>() > 0 && j["boundingBox"]["h"].get<float>() > 0) {
				e->addComponent<CBoundingBox>(
					Vec2(j["boundingBox"]["w"].get<float>(), j["boundingBox"]["h"].get<float>()),
					Vec2(j["boundingBox"]["relativeX"].get<float>(), j["boundingBox"]["relativeY"].get<float>()));
			}
		}
		if (j["type"] == "TEX_MAP") {

			auto e = entities.addEntity(j["name"]);

			std::vector<CGraphics::Tile> tiles;
			CGraphics::Tile tile;
			for (auto& t : j["tiles"]) {
				tile.size = Vec2(t["size"]["w"].get<float>(), t["size"]["h"].get<float>());
				tile.texturePos = Vec2(t["texturePos"]["x"].get<float>(), t["texturePos"]["y"].get<float>());
				tile.worldPos = Vec2(t["worldPos"]["x"].get<float>(), t["worldPos"]["y"].get<float>());
				tiles.push_back(tile);
			}
			auto& graphics = e->addComponent<CGraphics>(j["texture"], tiles);
			auto& transform = e->addComponent<CTransform>();
			transform.setScale(j["scale"]["w"].get<float>(), j["scale"]["h"].get<float>());
		}
	}
}

void GameEngine::run() {
	while (isRunning()) {
		sUserInput();
		update();
	}
	m_window.close();
}

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene) {
	auto deleteScene = m_scenes.find(m_currentScene);
	if (endCurrentScene && deleteScene != m_scenes.end()) {
		m_scenes.erase(deleteScene);
	}

	m_currentScene = sceneName;
	m_scenes[sceneName] = scene;
}

std::shared_ptr<Scene> GameEngine::getCurrentScene() {
	return m_scenes[m_currentScene];
}

std::shared_ptr<Scene> GameEngine::getScene(const std::string sceneName) {
	if (m_scenes.find(sceneName) == m_scenes.end()) {
		std::cout << "Scene not found\n";
		return NULL;
	}
	return m_scenes[sceneName];
}

void GameEngine::sUserInput() {
	sf::Event event;
	while (m_window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) m_running = false;
		
		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
			// not a key of a registered action
			if (getCurrentScene()->getActionMap().find(event.key.code) == getCurrentScene()->getActionMap().end()) { continue; }

			const Action::Type actionType = (event.type == sf::Event::KeyPressed) ? Action::START : Action::END;
			getCurrentScene()->doAction(Action(getCurrentScene()->getActionMap().at(event.key.code), actionType));
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(getWindow());

		if (event.type == sf::Event::MouseButtonPressed) {
			switch (event.mouseButton.button) {
			case sf::Mouse::Left: { getCurrentScene()->doAction(Action("MOUSE_LEFT", Action::START, mousePos)); break; }
				case sf::Mouse::Middle: { getCurrentScene()->doAction(Action("MOUSE_MIDDLE", Action::START, mousePos)); break; }
				case sf::Mouse::Right: { getCurrentScene()->doAction(Action("MOUSE_RIGHT", Action::START, mousePos)); break; }
				default: break;
			}
		}

		if (event.type == sf::Event::MouseButtonReleased) {
			switch (event.mouseButton.button) {
			case sf::Mouse::Left: { getCurrentScene()->doAction(Action("MOUSE_LEFT", Action::END, mousePos)); break; }
				case sf::Mouse::Middle: { getCurrentScene()->doAction(Action("MOUSE_MIDDLE", Action::END, mousePos)); break; }
				case sf::Mouse::Right: { getCurrentScene()->doAction(Action("MOUSE_RIGHT", Action::END, mousePos)); break; }
				default: break;
			}
		}

		if (event.type == sf::Event::MouseMoved) {
			getCurrentScene()->doAction(Action("MOUSE_MOVE", Action::START, mousePos));
		}

	}
}

std::string& GameEngine::getDialog(const std::string& key) {
	return m_dialogMap[key];
}

void GameEngine::quit() { 
	m_running = false;
}

void GameEngine::setDebugMode(bool debug) {
	m_debugMode = debug;
}

bool GameEngine::isDebugMode() const {
	return m_debugMode;
}

bool GameEngine::isRunning() {
	return m_running & m_window.isOpen();
}

void GameEngine::update() {
	getCurrentScene()->update();
}

sf::RenderWindow& GameEngine::getWindow() {
	return m_window;
}


Assets& GameEngine::getAssets() {
	return m_assets;
}


