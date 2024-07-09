#include "Physics.h"

const Vec2& Physics::getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
	Vec2& aPos = a->getComponent<CTransform>().getPos();
	CBoundingBox& aBoundingBox = a->getComponent<CBoundingBox>();
	Vec2& bPos = b->getComponent<CTransform>().getPos();
	CBoundingBox& bBoundingBox = b->getComponent<CBoundingBox>();

	Vec2 delta = Vec2(
		abs((aPos.x + aBoundingBox.relativePosition.x) - (bPos.x + bBoundingBox.relativePosition.x)), 
		abs((aPos.y + aBoundingBox.relativePosition.y) - (bPos.y + bBoundingBox.relativePosition.y)));
	return Vec2(
		aBoundingBox.halfSize.x + bBoundingBox.halfSize.x - delta.x,
		aBoundingBox.halfSize.y + bBoundingBox.halfSize.y - delta.y);
}

const Vec2& Physics::getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
	Vec2& aPrevPos = a->getComponent<CTransform>().getPrevPos();
	CBoundingBox& aBoundingBox = a->getComponent<CBoundingBox>();
	Vec2& bPrevPos = b->getComponent<CTransform>().getPrevPos();
	CBoundingBox& bBoundingBox = b->getComponent<CBoundingBox>();

	Vec2 delta = Vec2(
		abs((aPrevPos.x + aBoundingBox.relativePosition.x) - (bPrevPos.x + bBoundingBox.relativePosition.x)),
		abs((aPrevPos.y + aBoundingBox.relativePosition.y) - (bPrevPos.y + bBoundingBox.relativePosition.y)));
	return Vec2(
		aBoundingBox.halfSize.x + bBoundingBox.halfSize.x - delta.x,
		aBoundingBox.halfSize.y + bBoundingBox.halfSize.y - delta.y);
}