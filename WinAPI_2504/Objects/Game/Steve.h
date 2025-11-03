#pragma once

class Steve : public SphereCollider
{
private:
	const float GRAVITY = -98.0f;
	const float JUMP_POWER = 50.0f;
	const float MOVE_SPEED = 10.0f;

public:
	Steve();
	~Steve();

	void Update();
	void Render();

private:
	void Move();
	void Jump();
	void Gravity();

private:
	Vector3 velocity;
};