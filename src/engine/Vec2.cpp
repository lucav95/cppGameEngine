#include "Vec2.h"
#include <math.h>

Vec2::Vec2() {}

Vec2::Vec2(float xin, float yin) : x(xin), y(yin) {}

bool Vec2::operator == (const Vec2& vec) const {
	return x == vec.x && y == vec.y;
}

bool Vec2::operator != (const Vec2& vec) const {
	return x != vec.x || y != vec.y;
}

Vec2 Vec2::operator + (const Vec2& vec) const {
	return Vec2(x + vec.x, y + vec.y);
}

Vec2 Vec2::operator - (const Vec2& vec) const {
	return Vec2(x - vec.x, y - vec.y);
}

Vec2 Vec2::operator / (const float val) const {
	if (val == 0) {
		return Vec2(x, y);
	}
	return Vec2(x / val, y / val);
}

Vec2 Vec2::operator * (const float val) const {
	return Vec2(x * val, y * val);
}

void Vec2::operator += (const Vec2& vec) {
	x += vec.x;
	y += vec.y;
}

void Vec2::operator -= (const Vec2& vec) {
	x -= vec.x;
	y -= vec.y;
}

void Vec2::operator /= (const float val) {
	if (val == 0) return;
	x /= val;
	y /= val;
}

void Vec2::operator *= (const float val) {
	x *= val;
	y *= val;
}

float Vec2::dist(const Vec2& vec) const {
	return sqrt(pow(x - vec.x, 2) + pow(y - vec.y, 2));
}

