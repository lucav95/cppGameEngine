#pragma once

#include "Scene.h"
#include "../engine/MenuSystem.h"

class Scene_Fight : public Scene {

	enum AttackType {
		FIRE, ICE, POISON, LIGHTNING
	};

	struct DamagePointsAnimation {
		bool running = false;
		float gradient = 0;
		int points = 0;
		int opacity = 255;
		Vec2 pos = { 0, 0 };
	} m_damagePointsAnimation;
	
	std::shared_ptr<Entity>		m_player;
	std::shared_ptr<Entity>		m_enemy;
	std::shared_ptr<Entity>		m_attack;
	Vec2						m_viewPosition;
	MenuSystem					m_menu;
	bool						m_attackAnimationRunning = false;

	void						onEnd() override;
	void						init();
	void						getEnemy();
	void						fillDamagePointsAnimation(int damage);
	void						attack(const std::string& animationName, const std::string& attackName);

	void						renderStats();
	void						renderAttackAnimation();
	void						renderDamagePointsAnimation(int speed);
	void						renderHpText(int hp, int maxHp, const Vec2& pos);
	void						renderHpBar(float hp, float maxHp, const Vec2& pos);

public:
	Scene_Fight(GameEngine* game, std::shared_ptr<Entity>& player);

	void						update() override;
	void						sRender() override;
	void						sDoAction(const Action& action) override;
};