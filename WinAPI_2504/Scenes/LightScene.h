#pragma once

class LightScene : public Scene
{
public:
	LightScene();
	~LightScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Sphere* sphere;
	Plane* plane;
	Cube* cube;
};