#pragma once

#include "Scene.h"
#include "../engine/MenuSystem.h"

class Scene_Pause : public Scene {

	std::vector<std::string>	m_menuPoints;
	MenuSystem					m_menuSystem;

	void						onEnd() override;
	void						init();

public:
	Scene_Pause(GameEngine* game);

	void						update() override;
	void						sRender() override;
	void						sDoAction(const Action& action) override;
};