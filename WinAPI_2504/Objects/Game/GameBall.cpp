#include "Framework.h"

GameBall::GameBall()
{
	sphere = new Sphere();
	sphere->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Test.jfif");
	sphere->SetParent(this);
}

GameBall::~GameBall()
{
	delete sphere;
}

void GameBall::Update()
{
	UpdateWorld();
	sphere->UpdateWorld();
}

void GameBall::Render()
{
	if (!isActive) return;
	
	sphere->Render();
	Collider::Render();
}
