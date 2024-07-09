#pragma once

#include <memory>
#include "../engine/entity/EntityManager.h"
#include "../engine/Action.h"

// forward declaration
class GameEngine;

typedef std::map<int, std::string> IntStringMap;

class Scene {
protected:
	GameEngine*					m_game = nullptr;
	EntityManager				m_entities;
	int							m_currentFrame = 0;
	std::map<int, std::string>	m_actionMap;
	bool						m_paused = false;
	bool						m_hasEnded = false;

	void						setPaused(bool paused);
	virtual void				onEnd() = 0;

public:
	Scene();
	Scene(GameEngine* game);

	virtual void				update() = 0;
	virtual void				sDoAction(const Action& action) = 0;
	virtual void				sRender() = 0;

	int							getCurrentFrame() const;
	bool						hasEnded() const;
	const IntStringMap&			getActionMap() const;

	void						simulate(const int frames); // ?
	void						doAction(const Action& action);
	void						registerAction(
									int inputKey, 
									const std::string& actionName);
};