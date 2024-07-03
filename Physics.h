#pragma once

#include "Vec2.h"
#include "Entity.h"
#include <memory>

class Physics {
public:
	static Vec2 getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
	static Vec2 getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
};