#pragma once

class GameMath
{
public:
	static bool IsPointBetweenVectors(const Vector2& origin,
		const Vector2& leftPoint, const Vector2& rightPoint, const Vector2& testPoint);

	static int Random(const int& min, const int& max);
	static float Random(const float& min, const float& max);
	static Vector3 Random(const Vector3& min, const Vector3& max);

	static float Clamp(const float& value, const float& min, const float& max);

	static bool NearlyEqual(const float& s1, const float& s2);

	template<typename T>
	static T Lerp(const T& start, const T& end, float factor)
	{
		return start + (end - start) * factor;
	}
};