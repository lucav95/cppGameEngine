#include "Physics.h"

const Vec2& Physics::getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
	Vec2 delta = Vec2(
		abs(a->getComponent<CTransform>().getPos().x - b->getComponent<CTransform>().getPos().x),
		abs(a->getComponent<CTransform>().getPos().y - b->getComponent<CTransform>().getPos().y));
	return Vec2(
		a->getComponent<CBoundingBox>().halfSize.x + b->getComponent<CBoundingBox>().halfSize.x - delta.x,
		a->getComponent<CBoundingBox>().halfSize.y + b->getComponent<CBoundingBox>().halfSize.y - delta.y);
}

const Vec2& Physics::getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
	Vec2 delta = Vec2(
		abs(a->getComponent<CTransform>().getPrevPos().x - b->getComponent<CTransform>().getPrevPos().x),
		abs(a->getComponent<CTransform>().getPrevPos().y - b->getComponent<CTransform>().getPrevPos().y));
	return Vec2(
		a->getComponent<CBoundingBox>().halfSize.x + b->getComponent<CBoundingBox>().halfSize.x - delta.x,
		a->getComponent<CBoundingBox>().halfSize.y + b->getComponent<CBoundingBox>().halfSize.y - delta.y);
}