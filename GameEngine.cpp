#include "GameEngine.h"
#include "Scene_Main.h"
#include <iostream>
#include <fstream>

GameEngine::GameEngine(const std::string& configPath) {
	init(configPath);
}

void GameEngine::init(const std::string& configPath) {
	loadAssets(configPath);

	m_window.create(sf::VideoMode(1280, 720), "Game");
	m_window.setFramerateLimit(60);

	changeScene("main", std::make_shared<Scene_Main>(this));
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
	m_currentScene = sceneName;
	m_scenes[sceneName] = scene;
}

std::shared_ptr<Scene> GameEngine::getCurrentScene() {
	return m_scenes[m_currentScene];
}

void GameEngine::sUserInput() {
	sf::Event event;
	while (m_window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) m_running = false;
		
		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
			if (getCurrentScene()->getActionMap().find(event.key.code) == getCurrentScene()->getActionMap().end()) { continue; }

			const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";
			getCurrentScene()->doAction(Action(getCurrentScene()->getActionMap().at(event.key.code), actionType));
		}
	}
}

void GameEngine::quit() { 
	m_running = false;
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


