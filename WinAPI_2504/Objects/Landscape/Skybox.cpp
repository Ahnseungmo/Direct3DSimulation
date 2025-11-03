#include "Framework.h"
#include "Skybox.h"

Skybox::Skybox(wstring textureFile)
{
	material->SetShader(L"Landscape/Skybox.hlsl");

	//material->SetDiffuseMap(textureFile);
	cubeMap = Texture::Add(textureFile);

	rasterizerStates[0] = new RasterizerState();
	rasterizerStates[1] = new RasterizerState();
	rasterizerStates[1]->FrontCounterClockwise(true);

	depthStencilStates[0] = new DepthStencilState();
	depthStencilStates[1] = new DepthStencilState();
	depthStencilStates[1]->DepthEnable(false);
}

Skybox::~Skybox()
{
	for (UINT i = 0; i < 2; i++)
		delete rasterizerStates[i];

	for (UINT i = 0; i < 2; i++)
		delete depthStencilStates[i];
}

void Skybox::Render()
{
	rasterizerStates[1]->SetState();
	depthStencilStates[1]->SetState();

	cubeMap->PSSet(10);
	Sphere::Render();

	rasterizerStates[0]->SetState();
	depthStencilStates[0]->SetState();
}
