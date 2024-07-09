#pragma once

#include "Scene.h"
#include "Assets.h"

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine {
	SceneMap				m_scenes;
	Assets					m_assets;
	sf::RenderWindow		m_window;
	std::string				m_currentScene;
	bool					m_running = true;
	bool					m_debugMode = false;

	std::shared_ptr<Scene>	getCurrentScene();
	void					init(const std::string& configPath);
	void					update();
	void					sUserInput();
	void					loadAssets(const std::string& configPath);

public:
	GameEngine(const std::string& configPath);

	void					changeScene(
								const std::string& sceneName, 
								std::shared_ptr<Scene> scene, 
								bool endCurrentScene = false);
	Assets&					getAssets();
	sf::RenderWindow&		getWindow();
	void					run();
	void					quit();
	bool					isRunning();
	bool					isDebugMode() const;
	void					setDebugMode(bool debug);
};