#pragma once

class Skybox : public Sphere
{
public:
	Skybox(wstring textureFile);
	~Skybox();

	void Render();

private:
	Texture* cubeMap;

	RasterizerState* rasterizerStates[2];
	DepthStencilState* depthStencilStates[2];
};