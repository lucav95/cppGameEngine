#pragma once

#include "../Vec2.h"
#include "../Animation.h"
#include <SFML/Graphics.hpp>
#include <iostream>

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

	const Vec2& getTopLeftPos(float width, float height) const {
		return Vec2(
			pos.x - (width / 2),
			pos.y - (height / 2));
	}

	CTransform() {}
	CTransform(const Vec2& p, const Vec2& v, float a) 
		: pos(p), prevPos(p), velocity(v), angle(a) {}
};

class CBoundingBox : public Component {
public:
	Vec2 size;
	Vec2 halfSize;
	Vec2 relativePosition = Vec2(0, 0);

	const Vec2& getTopLeftPos(float x, float y) const {
		return Vec2(
			x - (size.x / 2) + relativePosition.x,
			y - (size.y / 2) + relativePosition.y);
	}

	CBoundingBox() {}
	CBoundingBox(const Vec2& s) : size(s), halfSize(s.x / 2, s.y / 2) {}
	CBoundingBox(const Vec2& s, const Vec2& rP) 
		: size(s), halfSize(s.x / 2, s.y / 2), relativePosition(rP) {}
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

class CGraphics : public Component {
public:
	Animation animation;
	std::string texture;
	CGraphics() {}
	CGraphics(const std::string& texture) : texture(texture) {}
	CGraphics(Animation& animation) : animation(animation) {}
};

class CState : public Component {

	std::string m_customState;
	int m_status = NONE;

public:
	enum Status { NONE, POISONED, BURNING, FROZEN, PARALIZED };

	void setCustomState(const std::string& state) {
		m_customState = state;
	}

	void setStatus(int status) {
		m_status = status;
	}

	const std::string& getCustomState() {
		return m_customState;
	}

	int getStatus() {
		return m_status;
	}

	CState() {}
	CState(const std::string& customState) : m_customState(customState) {}
	CState(int status) : m_status(status) {}
};

class CStats : public Component {
public:
	enum Type {
		NONE, FIRE, ICE, POISON, LIGHTNING
	};

	struct Attack {
		std::string name = "";
		int damage = 0;
		Type type = NONE;
	};

	std::vector<Attack> attacks;
	float maxHp;
	float hp;
	int weakness;
	int resistance;

	int damage(const Attack attack) {
		int attackPercent = (rand() % 25) + 75; // 75% - 100%

		float damage = attack.damage;
		if (attack.type == weakness && weakness != NONE) {
			damage += 5;
		}
		if (attack.type == resistance && resistance != NONE) {
			damage -= 5;
		}
		damage = damage <= 0 ? 1 : (damage / 100) * attackPercent;
		hp = damage > hp ? 0 : hp - damage;

		return damage;
	}

	int damage(int damage) {
		int damage_taken = damage > hp ? 0 : hp - damage;
		hp = damage_taken;
		return damage_taken;
	}

	void addAttack(const std::string& name, int damage, CStats::Type type) {
		attacks.push_back({name, damage, type});
	}

	const Attack& getAttack(const std::string& name) const {
		for (auto& attack : attacks) {
			if (attack.name == name) {
				return attack;
			}
		}
		std::cout << "Attack '" << name << "' not found\n";
		return { "", 0, NONE };
	}

	CStats() {
		maxHp = 100;
		hp = 100;
		weakness = NONE;
		resistance = NONE;
	}
	CStats(float maxHp, float health) : maxHp(maxHp), hp(health) {
		weakness = NONE;
		resistance = NONE;
	}
	CStats(float maxHp, float health, int weakness, int resistance) : maxHp(maxHp), hp(health), weakness(weakness), resistance(resistance) {}
};