#pragma once

#include "Vec2.h"
#include "Animation.h"
#include <SFML/Graphics.hpp>

class Component {
public:
	bool has = false;
};

class CTransform : public Component {
	Vec2 pos = { 0.0, 0.0 };
	Vec2 prevPos = { 0.0, 0.0 };
public:
	Vec2 scale = { 1.0, 1.0 };
	Vec2 velocity = { 0.0, 0.0 };
	float angle = 0;

	void setX(float x) {
		prevPos.x = pos.x;
		pos.x = x;
	}

	void setY(float y) {
		prevPos.y = pos.y;
		pos.y = y;
	}

	void setPosition(float x, float y) {
		prevPos.x = pos.x;
		prevPos.y = pos.y;
		pos.x = x;
		pos.y = y;
	}

	Vec2& getPos() { return pos; }
	Vec2& getPrevPos() { return prevPos; }

	CTransform() {}
	CTransform(const Vec2& p, const Vec2& v, float a) 
		: pos(p), prevPos(p), velocity(v), angle(a) {}
};

class CBoundingBox : public Component {
public:
	Vec2 size;
	Vec2 halfSize;
	CBoundingBox() {}
	CBoundingBox(const Vec2& s) 
		: size(s), halfSize(s.x / 2, s.y / 2) {}
};

class CLifespan : public Component {
public:
	int remaining = 0;
	int total = 0;
	CLifespan() {}
	CLifespan(int total)
		: remaining(total), total(total) {}
};

class CInput : public Component {
public:
	bool up = false;
	bool left = false;
	bool down = false;
	bool right = false;
	CInput() {}
};

class CAnimation : public Component {
public:
	Animation animation;
	CAnimation() {}
	CAnimation(Animation& animation) : animation(animation) {}
};

class CState : public Component {
public:
	std::string state;
	CState() {}
	CState(const std::string& state) : state(state) {}
};