#include "EntityManager.h"

EntityManager::EntityManager() {

}

void EntityManager::update() {
	for (auto e : m_toAdd) {
		m_entities.push_back(e);
		m_entityMap[e->getTag()].push_back(e);
	}
	m_toAdd.clear();
	
	m_entities.erase(
		std::remove_if(
			m_entities.begin(), 
			m_entities.end(), 
			[](const std::shared_ptr<Entity>& e) { return !e->isActive(); }),
		m_entities.end());

	updateEntityMap();
	m_totalEntities = m_entities.size();
}

void EntityManager::updateEntityMap() {
	std::vector<std::string> deletedEntryTags;
	for (auto& entry : m_entityMap) {
		entry.second.erase(
			std::remove_if(
				entry.second.begin(),
				entry.second.end(),
				[](const std::shared_ptr<Entity>& e) { return !e->isActive(); }),
			entry.second.end());

		if (entry.second.empty()) {
			deletedEntryTags.push_back(entry.first);
		}
	}
	for (auto tag : deletedEntryTags) {
		m_entityMap.erase(tag);
	}
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag) {
	// hacky
	auto e = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
	m_toAdd.push_back(e);
	return e;
}

EntityVec& EntityManager::getEntities() {
	return m_entities;
}

EntityVec& EntityManager::getEntities(const std::string& tag) {
	return m_entityMap[tag];
}
