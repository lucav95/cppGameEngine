#pragma once

#include <memory>
#include <string>
#include "Components.h"

class Entity {
	friend class EntityManager;

	const size_t					m_id = 0;
	const std::string				m_tag = "default";
	bool							m_active = true;

	Entity(size_t id, const std::string& tag);

public:
	std::shared_ptr<CTransform>		cTransform;
	std::shared_ptr<CShape>			cShape;
	std::shared_ptr<CCollision>		cCollision;
	std::shared_ptr<CInput>			cInput;
	std::shared_ptr<CScore>			cScore;
	std::shared_ptr<CLifespan>		cLifespan;
	std::shared_ptr<CBoundingBox>	cBoundingBox;

	const size_t					getId() const;
	const std::string&				getTag() const;
	bool							isActive() const;

	void							destroy();

};
