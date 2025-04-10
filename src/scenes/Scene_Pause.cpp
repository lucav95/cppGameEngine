#include "Scene_Pause.h"
#include "../engine/GameEngine.h"
#include <iostream>
#include "../engine/Physics.h"

Scene_Pause::Scene_Pause(GameEngine* game, std::shared_ptr<InventorySystem>& inventorySys) : Scene(game) {
	m_inventorySystem = inventorySys;
	init();
}

void Scene_Pause::init() {
	registerAction(sf::Keyboard::Up, "UP");
	registerAction(sf::Keyboard::Down, "DOWN");
	registerAction(sf::Keyboard::Escape, "CANCEL");
	registerAction(sf::Keyboard::Enter, "ACCEPT");

	std::vector<std::string> topMenuPoints = { "Inventory", "Save", "Quit" };
	std::vector<InventorySystem::Item> inventoryMenuPoints;
	
	for (auto& item : m_inventorySystem->getItems()) {
		inventoryMenuPoints.push_back(item);
	}
	
	auto windowSize = m_game->getWindow().getSize();
	Vec2 windowPos = Physics::getViewPosition(
		m_game->getWindow().getView(),
		Vec2(windowSize.x, windowSize.y));

	m_topMenu = std::make_shared<MenuSystem<std::string>>(
		m_game,
		Vec2(windowPos.x + 70, windowPos.y + 100),
		topMenuPoints,
		m_game->getAssets().getFont("pixelmix"));
	m_inventoryMenu = std::make_shared<MenuSystem<InventorySystem::Item>>(
		m_game,
		Vec2(windowPos.x + 70, windowPos.y + 100),
		inventoryMenuPoints,
		m_game->getAssets().getFont("pixelmix"));
}

void Scene_Pause::update() {
	sRender();
	m_currentFrame++;
}

void Scene_Pause::sRender() {
	m_game->getWindow().clear();

	// save current menu type with polymorphism ?
	if (m_currentMenu == MenuType::TOP) m_topMenu->render();
	if (m_currentMenu == MenuType::ITEMS) m_inventoryMenu->render();
	
	m_game->getWindow().display();
}

void Scene_Pause::sDoAction(const Action& action) {
	if (action.getName() == "UP" && action.getType() == Action::START) {
		if (m_currentMenu == MenuType::TOP) m_topMenu->updateCursorBy(-1);
		if (m_currentMenu == MenuType::ITEMS) m_inventoryMenu->updateCursorBy(-1);
	}
	if (action.getName() == "DOWN" && action.getType() == Action::START) {
		if (m_currentMenu == MenuType::TOP) m_topMenu->updateCursorBy(1);
		if (m_currentMenu == MenuType::ITEMS) m_inventoryMenu->updateCursorBy(1);
	}
	if (action.getName() == "CANCEL" && action.getType() == Action::START) {
		onEnd();
	}
	if (action.getName() == "ACCEPT" && action.getType() == Action::START) {

		if (m_currentMenu == MenuType::TOP) {
			switch (m_topMenu->getIndex()) {
			case TopMenuPoints::INVENTORY: m_currentMenu = MenuType::ITEMS; break;
			case TopMenuPoints::SAVE: break;
			case TopMenuPoints::QUIT: m_game->quit(); break;
			}
		}
	}
}

void Scene_Pause::onEnd() {
	m_game->changeScene("main", m_game->getScene("main"), true);
}
