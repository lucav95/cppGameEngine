#pragma once

#include "Vec2.h"
#include "../engine/entity/Entity.h"
#include "../engine/Vec2.h"
#include <memory>

class Physics {
public:
	static const Vec2& getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
	static const Vec2& getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
	// get the 0,0 position of the current view
	static const Vec2& getViewPosition(const sf::View& view, const Vec2& windowSize);
};