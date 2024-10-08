#pragma once

#include "../Scene.h"
#include "TextBoxSystem.h"

class Scene_Main : public Scene {

	Vec2					m_playerBox = Vec2(0.0f, 0.0f);
	Vec2					m_enemyBox = Vec2(0.0f, 0.0f);
	std::string				m_playerStandingTexture = "player";
	TextBoxSystem			m_textBoxSys = nullptr;
	// das ist kacki
	int						m_transitionOpacity = 0;
	bool					m_sceneChanged = false;
	bool					m_fight = false;

	sf::View				m_camera;

	std::shared_ptr<Entity> m_player;


	void					init();

	void					checkAnimationDirections(bool up, bool down, bool left, bool right);
	void					changeAnimation(const std::shared_ptr<Entity>& entity, const std::string& animationName);
	void					handlePlayerMovement(const Action& action, CInput& playerInput);

	void					renderBoundingBox(const std::shared_ptr<Entity>& entity);
	void					renderTransition(const Vec2& viewPosition);
	void					fight();

	void					cameraToPlayer();
	
	//systems
	void					sMovement(); 
	void					sCollision();

	void					spawnPlayer();
	void					spawnEntity(
								const Vec2& pos, 
								const Vec2& boundingBox,
								const std::string& name);

	void					onEnd() override;

public:
	Scene_Main(GameEngine* game);

	void					update() override;
	void					sRender() override;
	void					sDoAction(const Action& action) override;
};