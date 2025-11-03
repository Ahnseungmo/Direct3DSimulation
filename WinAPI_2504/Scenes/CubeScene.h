#pragma once

class CubeScene : public Scene
{
public:
	CubeScene();
	~CubeScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Quad* quad;
	Cube* cube;
	Plane* plane;
	Sphere* sphere;
};