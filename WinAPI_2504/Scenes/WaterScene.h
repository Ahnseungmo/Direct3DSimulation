#pragma once

class WaterScene : public Scene
{
public:
	WaterScene();
	~WaterScene();	

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	void CreateObjects();

private:
	Skybox* skybox;
	Sphere* sphere;
	Model* model;
	ModelAnimator* animator;

	//Water* water;

	Plane* plane;

	Shadow* shadow;

	//Reflection* reflection;
	//Refraction* refraction;
};