#pragma once

class GameBall : public SphereCollider
{
public:
	GameBall();
	~GameBall();

	void Update();
	void Render();

	Sphere* GetSphere() { return sphere; }

private:
	Sphere* sphere;
};