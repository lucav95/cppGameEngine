#pragma once

class Vec2 {
public:

	float x = 0;
	float y = 0;

	Vec2();
	Vec2(float xin, float yin);

	bool operator == (const Vec2& vec) const;
	bool operator != (const Vec2& vec) const;

	Vec2 operator + (const Vec2& vec) const;
	Vec2 operator - (const Vec2& vec) const;
	Vec2 operator / (const float val) const;
	Vec2 operator * (const float val) const;

	void operator += (const Vec2& vec);
	void operator -= (const Vec2& vec);
	void operator /= (const float val);
	void operator *= (const float val);

	float dist(const Vec2& vec) const;

};