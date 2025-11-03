#include "Framework.h"
#include "CubeScene.h"

CubeScene::CubeScene()
{
	quad = new Quad();
	quad->Load();
	quad->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Test.jfif");
	//quad->GetMaterial()->SetShader(L"Tutorial.hlsl");

	cube = new Cube();
	cube->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Landscape/Box.png");
	cube->GetMaterial()->SetShader(L"Light/DiffuseLight.hlsl");

	quad->SetParent(cube);

	plane = new Plane();
	plane->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Test.jfif");
	plane->GetMaterial()->SetShader(L"Light/DiffuseLight.hlsl");

	sphere = new Sphere();
	sphere->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Test.jfif");
	sphere->GetMaterial()->SetShader(L"Light/DiffuseLight.hlsl");
}

CubeScene::~CubeScene()
{
	delete quad;
	delete cube;
	delete plane;
	delete sphere;
}

void CubeScene::Update()
{
	cube->Rotate(Vector3::Up(), 1.0f * DELTA);

	quad->UpdateWorld();
	cube->UpdateWorld();
	plane->UpdateWorld();
	sphere->UpdateWorld();
}

void CubeScene::PreRender()
{
}

void CubeScene::Render()
{
	quad->Render();
	cube->Render();
	plane->Render();
	sphere->Render();
}

void CubeScene::PostRender()
{
}

void CubeScene::GUIRender()
{
	quad->Edit();
	cube->Edit();
	plane->Edit();
	sphere->Edit();
}
