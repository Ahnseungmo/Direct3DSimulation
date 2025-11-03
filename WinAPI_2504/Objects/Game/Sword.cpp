#include "Framework.h"

Sword::Sword() : BoxCollider(Vector3(0.2f, 0.5f, 3.5f))
{
	tag = "SwordCollider";
	
	model = new Model("Sword");
	model->SetParent(this);
	model->Load();

	Load();
}

Sword::~Sword()
{
	delete model;
}

void Sword::Update()
{
	UpdateWorld();	
	model->UpdateWorld();
}

void Sword::Render()
{
	BoxCollider::Render();
	model->Render();
}

void Sword::Edit()
{
	BoxCollider::Edit();
	model->Edit();
}
