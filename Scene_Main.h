#pragma once

#include "Scene.h"

class Scene_Main : public Scene {

	Vec2					m_playerBox = Vec2(0.0f, 0.0f);
	Vec2					m_enemyBox = Vec2(0.0f, 0.0f);
	std::string				m_playerStandingTexture = "player";

	std::shared_ptr<Entity> m_player;

	void					init();

	void					checkAnimationDirections(bool up, bool down, bool left, bool right);
	void					changeAnimation(std::shared_ptr<Entity> entity, const std::string& animationName, bool repeat);
	
	//systems
	void					sMovement(); 
	void					sCollision();

	void					spawnPlayer();
	void					spawnEnemy(float x, float y);

public:
	Scene_Main(GameEngine* game);

	void					update() override;
	void					sRender() override;
	void					sDoAction(const Action& action) override;
	void					onEnd() override;
};