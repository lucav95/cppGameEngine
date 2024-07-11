#pragma once

#include "Scene.h"

struct MenuPoint { float x; float y; float w; float h; };

class Scene_Menu : public Scene {

	void					init();
	void					sMouseInput();

	bool					m_startHover = false;
	MenuPoint				m_start;
	

public:
	Scene_Menu(GameEngine* game);

	void					update() override;
	void					sRender() override;
	void					sDoAction(const Action& action) override;
	void					onEnd() override;
};