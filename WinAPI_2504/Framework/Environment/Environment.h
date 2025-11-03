#pragma once

class Environment : public Singleton<Environment>
{
	friend class Singleton;

private:
	Environment();
	~Environment();

public:
	void Update();
	void Edit();

	void SetViewport(UINT width = SCREEN_WIDTH, UINT height = SCREEN_HEIGHT);
	void SetRender();
	void SetPostRender();

	void SetAlphaBlend(bool isAlpha);
	void SetAdditive();
	void SetAlphaToCoverage();
	void SetDepthEnable(bool isDepthEnable);
	void SetDepthWriteMask(D3D11_DEPTH_WRITE_MASK mask);

	LightBuffer::Light* AddLight();

	LightBuffer::Light* GetLight(UINT index) { return &lightBuffer->GetData()->lights[index]; }

	Camera* GetMainCamera() { return mainCamera; }
	Matrix GetPerspective() { return perspective; }

private:
	void CreateProjection();
	void CreateSamplerState();
	void CreateBlendState();	
	void CreateStats();

	void EditLight(LightBuffer::Light& light);

private:
	bool isWireFrame = false;

	Matrix perspective;
	Matrix orthographic;

	class Grid* grid;
	Camera* mainCamera;
	MatrixBuffer* projectionBuffer;
	MatrixBuffer* uiViewBuffer;
	LightBuffer* lightBuffer;

	ID3D11SamplerState* samplerState;	

	RasterizerState* rasterizerState[2];	
	BlendState* blendState[3];
	DepthStencilState* depthStencilState[3];
};