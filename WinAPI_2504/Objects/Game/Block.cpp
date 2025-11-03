#include "Framework.h"

Block::Block()
{
	cube = new Cube();
	cube->SetParent(this);
}

Block::~Block()
{
	delete cube;
}

void Block::Update()
{
	BoxCollider::UpdateWorld();
	cube->UpdateWorld();
}

void Block::Render()
{
	if (!isActive) return;

	cube->Render();
	BoxCollider::Render();
}
