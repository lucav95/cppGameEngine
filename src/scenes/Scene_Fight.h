#pragma once

#include "Scene.h"

class Scene_Fight : public Scene {

	std::shared_ptr<Entity>		m_player;
	Vec2						m_viewPosition;
	int							m_currentFrame;

	void						onEnd() override;
	void						init();

	void						renderStats();

public:
	Scene_Fight(GameEngine* game, std::shared_ptr<Entity>& player);

	void						update() override;
	void						sRender() override;
	void						sDoAction(const Action& action) override;
};