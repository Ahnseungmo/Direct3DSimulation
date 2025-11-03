#pragma once

struct Vector2 : public Float2
{
	Vector2() : Float2(0, 0)
	{}
	Vector2(float x, float y) : Float2(x, y)
	{}
	Vector2(Float2 value) : Float2(value)
	{
	}

	Vector2 operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}
	Vector2 operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}
	Vector2 operator*(const Vector2& other) const
	{
		return Vector2(x * other.x, y * other.y);
	}
	Vector2 operator/(const Vector2& other) const
	{
		return Vector2(x / other.x, y / other.y);
	}

	Vector2 operator*(const float& scala) const
	{
		return Vector2(x * scala, y * scala);
	}

	void operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
	}
	void operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
	}
	void operator*=(const Vector2& other)
	{
		x *= other.x;
		y *= other.y;
	}
	void operator/=(const Vector2& other)
	{
		x/= other.x;
		y/= other.y;
	}
	void operator*=(const float& scala)
	{
		x *= scala;
		y *= scala;
	}

	Vector2 operator*(const Matrix& mat) const
	{
		Vector4 coord = XMLoadFloat2(this);

		coord = XMVector2TransformCoord(coord, mat);

		Vector2 result;
		XMStoreFloat2(&result, coord);

		return result;
	}

	float Magnitude() const	{ return sqrt(x * x + y * y); }
	float SqrMagnitude() const { return x * x + y * y; }

	void Normalize()
	{
		float length = Magnitude();

		if (length > 0)
		{
			x /= length;
			y /= length;
		}
	}

	Vector2 GetNormalized() const
	{
		float length = Magnitude();

		return Vector2(x / length, y / length);
	}	

	static const Vector2 Zero() { return Vector2(0, 0); }
	static const Vector2 One() { return Vector2(1, 1); }
		   
	static const Vector2 Right() { return Vector2(1, 0); }
	static const Vector2 Left() { return Vector2(-1, 0); }
	static const Vector2 Down() { return Vector2(0, -1); }
	static const Vector2 Up() { return Vector2(0, 1); }

	static float Distance(const Vector2& v1, const Vector2& v2)
	{
		return (v1 - v2).Magnitude();
	}

	static float Cross(const Vector2& v1, const Vector2& v2)
	{ return v1.x * v2.y - v1.y * v2.x; }

	static float Dot(const Vector2& v1, const Vector2& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}
};