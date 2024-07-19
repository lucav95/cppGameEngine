#pragma once

#include "Scene.h"

class Scene_Pause : public Scene {

	std::vector<std::string>	m_menuPoints;
	float						m_cursorPosY = 100;
	int							m_menuIndex = 0;
	Vec2						m_windowPos;

	void						onEnd() override;
	void						init();

public:
	Scene_Pause(GameEngine* game);

	void						update() override;
	void						sRender() override;
	void						sDoAction(const Action& action) override;
};