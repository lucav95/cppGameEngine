#include "EntityManager.h"

EntityManager::EntityManager() {

}

void EntityManager::update() {
	for (auto e : m_toAdd) {
		m_entities.push_back(e);
		m_entityMap[e->getTag()].push_back(e);
	}
	m_toAdd.clear();
	
	// does m_entityMap have to be considered?
	// todo: m_totalEntities HAS TO BE UPDATED
	m_entities.erase(
		std::remove_if(
			m_entities.begin(), 
			m_entities.end(), 
			[](const std::shared_ptr<Entity>& e) { return !e->isActive(); }),
		m_entities.end());
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
