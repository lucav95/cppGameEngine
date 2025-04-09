#pragma once

#include "Scene.h"
#include "../systems/menu_system/MenuSystem.h"
#include "../systems/InventorySystem.h"

class Scene_Pause : public Scene {

	enum TopMenuPoints {
		INVENTORY, SAVE, QUIT
	};

	enum MenuType {
		TOP, ITEMS
	};

	int															m_currentMenu = MenuType::TOP;
	
	std::shared_ptr<InventorySystem>							m_inventorySystem;

	std::shared_ptr<MenuSystem<std::string>>					m_topMenu;
	std::shared_ptr<MenuSystem<InventorySystem::Item>>			m_inventoryMenu;

	void						onEnd() override;
	void						init();

public:
	Scene_Pause(GameEngine* game, std::shared_ptr<InventorySystem>& inventorySys);

	void						update() override;
	void						sRender() override;
	void						sDoAction(const Action& action) override;
};