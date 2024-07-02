#pragma once

#include "Vec2.h"
#include <SFML/Graphics.hpp>

class CTransform {
public:
	Vec2 pos = { 0.0, 0.0 };
	Vec2 prevPos = { 0.0, 0.0 };
	Vec2 scale = { 1.0, 1.0 };
	Vec2 velocity = { 0.0, 0.0 };
	float angle = 0;

	CTransform(const Vec2& p, const Vec2& v, float a) 
		: pos(p), velocity(v), angle(a) {}
};

class CBoundingBox {
public:
	Vec2 size;
	Vec2 halfSize;
	CBoundingBox(const Vec2& s) 
		: size(s), halfSize(s.x / 2, s.y / 2) {}
};

class CShape {
	// muss noch generalisiert werden
public:
	sf::CircleShape circle;

	CShape(float radius, int points, const sf::Color& fill, const sf::Color& outline, float thickness)
		: circle(radius, points) {
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);
	}
};

// todo: HAS TO BE ALTERED WHEN CIRCLES ARE TOSSED
class CCollision {
public:
	float radius = 0;
	CCollision(float r)
		: radius(r) {}
};

class CScore {
public:
	int score = 0;
	CScore(int s)
		: score(s) {}
};

class CLifespan {
public:
	int remaining = 0;
	int total = 0;
	CLifespan(int total)
		: remaining(total), total(total) {}
};

// do I still need that?
class CInput {
public:
	bool up = false;
	bool left = false;
	bool down = false;
	bool right = false;
	CInput() {}
};

