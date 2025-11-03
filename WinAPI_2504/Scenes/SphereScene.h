#pragma once

class SphereScene : public Scene
{
private:
	const UINT BALL_COUNT = 50;
	const float SPAWN_TIME = 1.0f;
	const float MAX_RANGE = 20.0f;

public:
	SphereScene();
	~SphereScene();	

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	void Spawn();
	void LoadTextures();

	void CheckCollision();

private:
	float spawnTimer = 0.0f;
	RayHit hit;

	Quad* crossHair;

	vector<GameBall*> balls;

	vector<Texture*> ballTextures;
};