#include "Framework.h"
#include "Environment.h"

Environment::Environment()
{
	grid = new Grid();
	mainCamera = new Camera();
	mainCamera->Load();
	uiViewBuffer = new MatrixBuffer();
	lightBuffer = new LightBuffer();

	CreateProjection();
    CreateSamplerState();
	CreateBlendState();
	CreateStats();
}

Environment::~Environment()
{
	delete grid;
	mainCamera->Save();
	delete mainCamera;
	delete projectionBuffer;
	delete uiViewBuffer;
	delete lightBuffer;

	delete rasterizerState[0];
	delete rasterizerState[1];

    samplerState->Release();
	//alphaBlendState->Release();
}

void Environment::Update()
{
	if (Input::Get()->IsKeyDown(VK_F1))
	{
		Collider::SwitchDraw();
	}

	if (Input::Get()->IsKeyDown(VK_F2))
	{
		isWireFrame = !isWireFrame;		
	}

	mainCamera->Update();
}

void Environment::Edit()
{
	mainCamera->Edit();

	ImGui::Text("Light Option");

	ImGui::ColorEdit3("AmbientLight", (float*)&lightBuffer->GetData()->ambientLight);
	ImGui::ColorEdit3("AmbientCeil", (float*)&lightBuffer->GetData()->ambientCeil);

	if (ImGui::Button("Add"))
		lightBuffer->GetData()->lightCount++;	

	for (UINT i = 0; i < lightBuffer->GetData()->lightCount; i++)
	{
		string name = "Light " + to_string(i);

		if (ImGui::TreeNode(name.c_str()))
		{
			EditLight(lightBuffer->GetData()->lights[i]);
			ImGui::TreePop();
		}
	}
}

void Environment::SetViewport(UINT width, UINT height)
{
	D3D11_VIEWPORT viewport;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	DC->RSSetViewports(1, &viewport);	
}

void Environment::SetRender()
{
	mainCamera->SetView();

	SetViewport();

	projectionBuffer->Set(perspective);
	projectionBuffer->SetVS(2);

	rasterizerState[isWireFrame]->SetState();
	blendState[0]->SetState();
	depthStencilState[0]->SetState();

	//grid->Render();

	lightBuffer->SetPS(1);
}

void Environment::SetPostRender()
{
	uiViewBuffer->SetVS(1);	

	projectionBuffer->Set(orthographic);
	projectionBuffer->SetVS(2);

	blendState[1]->SetState();
	depthStencilState[1]->SetState();
}

void Environment::SetAlphaBlend(bool isAlpha)
{
	blendState[2]->Alpha(isAlpha);
	blendState[2]->AlphaToCoverage(false);
	blendState[2]->SetState();
}

void Environment::SetAdditive()
{
	blendState[2]->Additive();
	blendState[2]->AlphaToCoverage(false);
	blendState[2]->SetState();
}

void Environment::SetAlphaToCoverage()
{
	blendState[2]->AlphaToCoverage(true);
	blendState[2]->SetState();
}

LightBuffer::Light* Environment::AddLight()
{
	lightBuffer->GetData()->lightCount++;

	return &lightBuffer->GetData()->lights[lightBuffer->GetData()->lightCount - 1];
}

void Environment::CreateProjection()
{   
    projectionBuffer = new MatrixBuffer();

    //Orthographic : 원근감이 없는 직육면체의 절두체를 형성하는 투영변환
    orthographic = XMMatrixOrthographicOffCenterLH(0.0f,
		SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, -1.0f, 1.0f);
	//Perspective : 원근감이 있는 투영변환
	perspective = XMMatrixPerspectiveFovLH(PI * 0.25f,
		SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.f);
    
    projectionBuffer->SetVS(2);
}

void Environment::CreateSamplerState()
{
	D3D11_SAMPLER_DESC desc = {};
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	//LOD(Level Of Detail) : 카메라와의 거리에 따라서 퀄리티를 나누는 기술

	DEVICE->CreateSamplerState(&desc, &samplerState);

	DC->PSSetSamplers(0, 1, &samplerState);
}

void Environment::CreateBlendState()
{
	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Alpha(true);
	blendState[2] = new BlendState();
}

void Environment::SetDepthEnable(bool isDepthEnable)
{
	depthStencilState[2]->DepthEnable(isDepthEnable);
	depthStencilState[2]->SetState();
}

void Environment::SetDepthWriteMask(D3D11_DEPTH_WRITE_MASK mask)
{
	depthStencilState[2]->DepthWriteMask(mask);
	depthStencilState[2]->SetState();
}

void Environment::CreateStats()
{
	rasterizerState[0] = new RasterizerState();
	rasterizerState[1] = new RasterizerState();
	rasterizerState[1]->FillMode(D3D11_FILL_WIREFRAME);

	depthStencilState[0] = new DepthStencilState();
	depthStencilState[1] = new DepthStencilState();
	depthStencilState[1]->DepthEnable(false);
	depthStencilState[2] = new DepthStencilState();
}

void Environment::EditLight(LightBuffer::Light& light)
{
	ImGui::Checkbox("Active", (bool*)&light.active);

	const char* items[] = { "Directional", "Point", "Spot", "Capsule" };
	ImGui::Combo("Type", (int*)&light.type, items, IM_ARRAYSIZE(items));

	ImGui::ColorEdit3("Color", (float*)&light.color);
	ImGui::SliderFloat3("Direction", (float*)&light.direction, -1, 1);
	ImGui::DragFloat3("Position", (float*)&light.position);
	ImGui::SliderFloat("Range", &light.range, 0, 500);
	ImGui::SliderFloat("Inner", &light.inner, 0, 180);
	ImGui::SliderFloat("Outer", &light.outer, 0, 180);
	ImGui::DragFloat("Length", &light.length, 0.1f, 10);
}
