#include "Vector2.h"
#include "MathUtils.h"
#include <cmath>

const Vector2 Vector2::Zero(0.0f, 0.0f);
const Vector2 Vector2::UnitX(1.0f, 0.0f);
const Vector2 Vector2::UnitY(0.0f, 1.0f);

float Vector2::Length() const
{
    return std::sqrt(LengthSq());
}

float Vector2::LengthSq() const
{
    return (x * x + y * y);
}

void Vector2::Normalize()
{
    float len = this->Length();
    this->x /= len;
    this->y /= len;
}

Vector2 Vector2::Normalized() const
{
    Vector2 result = *this;
    result.Normalize();
    return result;
}

float Vector2::Dot(const Vector2& a, const Vector2& b)
{
    return (a.x * b.x + a.y * b.y);
}

Vector2 Vector2::Clamp(const Vector2& v, const Vector2& a, const Vector2& b)
{
    float x = Math::Clamp(v.x, a.x, b.x);
    float y = Math::Clamp(v.y, a.y, b.y);
    return Vector2(x, y);
}
