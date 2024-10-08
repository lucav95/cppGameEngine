#include "GameEngine.h"
#include "../scenes/Scene_Menu.h"
#include <iostream>
#include <fstream>
#include <sstream>

GameEngine::GameEngine(const std::string& configPath, const std::string& dialogPath) {
	init(configPath, dialogPath);
}

void GameEngine::init(const std::string& configPath, const std::string& dialogPath) {
	loadAssets(configPath);
	loadDialog(dialogPath);

	m_window.create(sf::VideoMode(1280, 720), "Game", sf::Style::Close);
	m_window.setFramerateLimit(60);
	
	changeScene("menu", std::make_shared<Scene_Menu>(this));
}

void GameEngine::loadAssets(const std::string& configPath) {
	std::ifstream fin(configPath);

	std::string header;
	std::string name;
	std::string path;
	std::string aniFrameCount;
	std::string aniSpeed;
	Animation animation;

	while (fin >> header) {
		if (header == "TEX") {
			fin >> path >> name;
			m_assets.addTexture(name, path);
		}
		if (header == "ANI") {
			fin >> path >> name >> aniFrameCount >> aniSpeed;
			m_assets.addTexture(name, path);
			animation = Animation(name, m_assets.getTexture(name), std::stoi(aniFrameCount), std::stoi(aniSpeed));
			m_assets.addAnimation(name, animation);
		}
		if (header == "FON") {
			fin >> path >> name;
			m_assets.addFont(name, path);
		}
	}
	fin.close();
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
	return m_running & m_window.isOpen(); // bit-wise comparison?
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


