#include "Framework.h"

InteriorObject::InteriorObject()
	: model(nullptr)
{
}

InteriorObject::InteriorObject(string meshName)
{
    model = new Model(meshName);
    model->SetParent(this);
}

InteriorObject::~InteriorObject()
{
    delete model;
}

void InteriorObject::Update()
{
    if (!isInstanced)
    {
        UpdateWorld();
        model->UpdateWorld();
    }
}

void InteriorObject::Render()
{
    if (!isInstanced)
    {
        BoxCollider::Render();
        model->Render();
    }
}
