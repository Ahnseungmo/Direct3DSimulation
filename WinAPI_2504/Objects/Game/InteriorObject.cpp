#include "Framework.h"

InteriorObject::InteriorObject()
	: model(nullptr)
{

}

InteriorObject::InteriorObject(string meshName)
{
    model = new Model(meshName);
    model->SetParent(this);
    FitSize(model->GetMeshes());
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
    BoxCollider::Render();
    if (!isInstanced)
    {
        BoxCollider::Render();
        model->Render();
    }
}
void InteriorObject::Edit()
{
    BoxCollider::Edit();
    model->Edit();
}
