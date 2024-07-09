#include "Scene.h"

Scene::Scene() {}

Scene::Scene(GameEngine* game) {
	m_game = game;
}
void Scene::setPaused(bool paused) {
	m_paused = paused;
}

int Scene::getCurrentFrame() const {
	return m_currentFrame;
}

bool Scene::hasEnded() const {
	return m_hasEnded;
}

const std::map<int, std::string>& Scene::getActionMap() const {
	return m_actionMap;
}

void Scene::simulate(const int frames) { }

void Scene::doAction(const Action& action) {
	sDoAction(action);
}

void Scene::registerAction(int inputKey, const std::string& actionName) {
	m_actionMap[inputKey] = actionName;
}