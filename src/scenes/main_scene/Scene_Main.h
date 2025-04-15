#pragma once

#include "../Scene.h"
#include "../../systems/TextBoxSystem.h"
#include "../../systems/InventorySystem.h"

class Scene_Main : public Scene {
	
	TextBoxSystem						m_textBoxSys = nullptr;
	std::shared_ptr<InventorySystem>	m_inventorySys;

	int									m_transitionOpacity = 0;

	bool								m_sceneChanged = false;
	bool								m_fight = false;

	sf::View							m_camera;

	std::shared_ptr<Entity>				m_player;

	sf::Shader							m_shader;
	sf::Clock							m_clock;

	void					init();

	void					loadMap(const std::string& path);
	void					correctAnimationDirections(bool up, bool down, bool left, bool right);
	void					changeAnimation(const std::shared_ptr<Entity>& entity, const std::string& animationName);
	void					handlePlayerMovement(const Action& action, CInput& playerInput);

	void					renderEntity(const std::shared_ptr<Entity>& e);

	void					renderPlayer(const std::shared_ptr<Entity>& e);
	void					renderBoundingBox(const std::shared_ptr<Entity>& e);
	void					renderTransitionAnimation(bool fadeOut); // fadeOut if animation is fading out or in
	void					fight();

	void					cameraToPlayer();
	void					updateZIndexes();
	
	//systems
	void					sMovement(); 
	void					sCollision();

	void					spawnPlayer();

	void					onEnd() override;

public:
	Scene_Main(GameEngine* game);

	void					update() override;
	void					sRender() override;
	void					sDoAction(const Action& action) override;
};