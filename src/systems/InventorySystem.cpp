#include "InventorySystem.h"

const InventorySystem::Item& InventorySystem::getItemFromRegister(const std::string& name) const {
	for (const Item& i : m_itemRegister) {
		if (i.name == name) return i;
	}
	std::cout << "No such Item in register\n";
	return { "NULL", 0 };
}

const std::vector<InventorySystem::Item>& InventorySystem::getItems() const {
	return m_items;
}

const InventorySystem::Item& InventorySystem::getItem(const std::string& name) const {
	for (const InventorySystem::Item& i : m_items) {
		if (i.name == name) return i;
	}
	// name ist leer?
	return { "NULL", 0 };
}

void InventorySystem::addItem(const std::string& name, int amount) {
	Item registerItem = getItemFromRegister(name);
	if (registerItem.name == "NULL") return;
	
	for (Item& i : m_items) {
		if (i.name == name) {
			i.add(amount);
			return;
		}
	}
	registerItem.count = amount;
	m_items.push_back(registerItem);
}

void InventorySystem::removeItem(const std::string& name, int amount) {
	auto iterator = std::find_if(m_items.begin(), m_items.end(), [&](const InventorySystem::Item& item) {
		return item.name == name;
	});
	if (iterator != m_items.end()) {

		int index = std::distance(m_items.begin(), iterator);
		InventorySystem::Item& foundItem = m_items[index];

		if (foundItem.count > amount) {
			foundItem.count -= amount;
		}
		else {
			m_items.erase(m_items.begin() + index);
			std::cout << "erased " + name + "\n";
		}
	}
}
