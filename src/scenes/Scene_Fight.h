#pragma once

#include "Scene.h"

class Scene_Fight : public Scene {

	std::shared_ptr<Entity>		m_player;
	Vec2						m_viewPosition;

	void						onEnd() override;
	void						init();
	void						getEnemy();

	void						renderStats();
	void						renderHpText(int hp, int maxHp, const Vec2& pos);
	void						renderHpBar(float hp, float maxHp, const Vec2& pos);

public:
	Scene_Fight(GameEngine* game, std::shared_ptr<Entity>& player);

	void						update() override;
	void						sRender() override;
	void						sDoAction(const Action& action) override;
};