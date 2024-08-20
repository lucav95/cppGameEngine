#pragma once

#include <memory>
#include <string>
#include "Components.h"

class Entity {
	friend class EntityManager;

	typedef std::tuple<
		CTransform,
		CLifespan,
		CInput,
		CBoundingBox,
		CGraphics,
		CState,
		CStats
	> ComponentTuple;

	const size_t					m_id = 0;
	const std::string				m_tag = "default";
	bool							m_active = true;
	ComponentTuple					m_components;

	// only access via EntityManager
	Entity(const size_t id, const std::string& tag);

public:

	const size_t					getId() const;
	const std::string&				getTag() const;
	bool							isActive() const;
	void							destroy();

	template<typename T> bool hasComponent() {
		return getComponent<T>().has;
	}

	template<typename T, typename... TArgs> T& addComponent(TArgs&&... mArgs) {
		auto& component = getComponent<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.has = true;
		return component;
	}

	template<typename T> T& getComponent() {
		return std::get<T>(m_components);
	}

	template<typename T> void removeComponent() {
		getComponent<T>() = T();
	}
};
