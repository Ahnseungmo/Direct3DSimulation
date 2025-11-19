#include "Framework.h"
Item::Item(string id)
    : id(id)
{
}

Item::~Item()
{
    for (InteriorObject* obj : parts)
        delete obj;
}

void Item::AddPart(string meshName, Vector3 pos, Vector3 rot, Vector3 scale)
{
    InteriorObject* obj = new InteriorObject(meshName);
    obj->SetLocalPosition(pos);
    obj->SetLocalRotation(rot);
    obj->SetLocalScale(scale);

    parts.push_back(obj);
}

void Item::Update()
{
    for (auto obj : parts)
        obj->Update();
}

void Item::Render()
{
    for (auto obj : parts)
        obj->Render(); // Instancing이 켜져 있으면 actual mesh는 InteriorManager가 그려줌
}

void Item::Edit()
{
    for (auto obj : parts)
        obj->Edit();
}
