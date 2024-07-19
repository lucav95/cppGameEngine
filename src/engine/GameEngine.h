#pragma once

#include "../scenes/Scene.h"
#include "Assets.h"

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;
typedef std::map<std::string, std::string> DialogMap;

class GameEngine {
	SceneMap				m_scenes;
	Assets					m_assets;
	sf::RenderWindow		m_window;
	std::string				m_currentScene;
	bool					m_running = true;
	bool					m_debugMode = false;
	DialogMap				m_dialogMap;

	std::shared_ptr<Scene>	getCurrentScene();
	void					init(const std::string& configPath, const std::string& dialogPath);
	void					update();
	void					sUserInput();
	void					loadAssets(const std::string& configPath);
	void					loadDialog(const std::string& dialogPath);

public:
	GameEngine(const std::string& configPath, const std::string& dialogPath);

	void					changeScene(
								const std::string& sceneName, 
								std::shared_ptr<Scene> scene, 
								bool endCurrentScene = false);
	std::shared_ptr<Scene>	getScene(const std::string sceneName);
	Assets&					getAssets();
	sf::RenderWindow&		getWindow();
	std::string&			getDialog(const std::string& key);
	void					run();
	void					quit();
	bool					isRunning();
	bool					isDebugMode() const;
	void					setDebugMode(bool debug);
};