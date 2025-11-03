#include "Framework.h"

bool Collider::isDraw = true;

Collider::Collider() : GameObject(L"Basic/Collider.hlsl")
{    
	mesh = new Mesh<Vertex>();

    SetColor(0, 1, 0);
}

bool Collider::IsCollision(Collider* collider, Vector2* overlap)
{
    if (!IsActive()) return false;
    if (!collider->IsActive()) return false;

    switch (collider->type)
    {
	case Type::Box:
		return IsBoxCollision((BoxCollider*)collider);
	case Type::Sphere:
		return IsSphereCollision((SphereCollider*)collider);
	case Type::Capsule:
		return IsCapsuleCollision((CapsuleCollider*)collider);
    }

    return false;
}

void Collider::Render()
{
	if (!isDraw) return;
    if (!isActive) return;

    worldBuffer->Set(world);
    worldBuffer->SetVS(0);    

    material->Set();

    mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}