#pragma once

#include "Scene.h"

typedef std::map<std::shared_ptr<Entity>, Vec2> EntityVec2Map;

class Scene_Main : public Scene {

	Vec2					m_playerBox = Vec2(0.0f, 0.0f);
	Vec2					m_enemyBox = Vec2(0.0f, 0.0f);
	std::string				m_playerAnimation = "player_animation";
	std::string				m_playerStandingTexture = "player";

	std::shared_ptr<Entity> m_player;

	EntityVec2Map			m_enemyLastOverlap;

	void					init();

	void					checkAnimationDirections(bool up, bool down, bool left, bool right);
	
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