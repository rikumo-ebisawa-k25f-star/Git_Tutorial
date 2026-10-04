#pragma once

class Vector2 final
{
public:
	static const Vector2 Zero;
	static const Vector2 UnitX;
	static const Vector2 UnitY;

public:
	float x;
	float y;

public:
	Vector2() : x(0.0f), y(0.0f) {}
	Vector2(float s) : x(s), y(s) {}
	Vector2(float sx, float sy) : x(sx), y(sy) {}
	~Vector2() = default;

public:
	Vector2 operator + (const Vector2& v) const
	{
		return Vector2(x + v.x, y + v.y);
	}
	Vector2 operator - (const Vector2& v) const
	{
		return Vector2(x - v.x, y - v.y);
	}
	Vector2 operator * (const Vector2& v) const
	{
		return Vector2(x * v.x, y * v.y);
	}
	void operator += (const Vector2& v)
	{
		this->x += v.x;
		this->y += v.y;
	}
	void operator -= (const Vector2& v)
	{
		this->x -= v.x;
		this->y -= v.y;
	}
	void operator *= (const Vector2& v)
	{
		this->x *= v.x;
		this->y *= v.y;
	}
	Vector2 operator - () const
	{
		return Vector2(-x, -y);
	}

public:
	float Length() const;
	float LengthSq() const;
	void Normalize();
	Vector2 Normalized() const;

public:
	static float Dot(const Vector2& a, const Vector2& b);
	static Vector2 Clamp(const Vector2& v, const Vector2& a, const Vector2& b);

};
