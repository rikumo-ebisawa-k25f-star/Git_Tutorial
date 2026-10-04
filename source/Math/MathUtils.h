#pragma once

#include <algorithm>
#include <random>

namespace Math
{
	static constexpr float Pi = 3.14159265359f;
	static constexpr float TwoPi = Pi * 2.0f;
	static constexpr float PiOverTwo = Pi * 0.5f;
	static constexpr float Deg2Rad = Pi / 180.0f;
	static constexpr float Rad2Deg = 180.0f / Pi;

	static inline float ToRadians(float d)
	{
		return d * Deg2Rad;
	}
	static inline float ToDegrees(float r)
	{
		return r * Rad2Deg;
	}

	static inline float Clamp(float value, float min, float max)
	{
		if (value < min)
		{
			return min;
		}
		if (value > max)
		{
			return max;
		}
		return value;
	}
	static inline float Clamp01(float value)
	{
		return Clamp(value, 0.0f, 1.0f);
	}
	static inline float Lerp(float a, float b, float t)
	{
		return a + (b - a) * Clamp01(t);
	}

	static inline float RandomRange(float min, float max)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(min, max);
		return dis(gen);
	}
	static inline int RandomRange(int min, int max)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(min, max);
		return dis(gen);
	}
}
