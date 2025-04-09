#pragma once
#include <vector>
#include <string>
#include <iostream>


class InventorySystem {

public:

	struct Item {
		std::string name;
		int count;

		void add(int amount) { count += amount; }
	};
	
	const std::vector<Item>&	getItems() const;
	const Item&					getItem(const std::string& name) const;
	void						addItem(const std::string& name, int amount);
	void						removeItem(const std::string& name, int amount);

private:
	
	std::vector<Item>		m_items;
	const std::vector<Item> m_itemRegister {
		{ "Health Potion", 1 },
		{ "Mana Potion", 1}
	};

	const Item&					getItemFromRegister(const std::string& name) const;

};