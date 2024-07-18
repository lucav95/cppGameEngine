#pragma once

#include "Scene.h"

class Scene_Pause : public Scene {

	sf::View					m_camera;
	std::vector<std::string>	m_menuPoints;
	void						onEnd() override;
	void						init();

public:
	Scene_Pause(GameEngine* game);

	void						update() override;
	void						sRender() override;
	void						sDoAction(const Action& action) override;
};