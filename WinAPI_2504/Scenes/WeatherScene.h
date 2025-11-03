#pragma once

class WeatherScene : public Scene
{
public:
	WeatherScene();
	~WeatherScene();	

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Collider* collider;
	//Particle* particle;
	ParticleSystem* particle;
};