#include "Framework.h"
#include "CollisionScene.h"

CollisionScene::CollisionScene()
{
	colliders.push_back(new CapsuleCollider());
	colliders.back()->SetTag("CapsuelCollider1");

	//colliders.push_back(new BoxCollider());
	//colliders.back()->SetTag("BoxCollider1");

	colliders.push_back(new SphereCollider());
	colliders.back()->SetTag("SphereCollider1");

	//colliders.push_back(new BoxCollider());
	//colliders.back()->SetTag("BoxCollider2");

	//colliders.push_back(new SphereCollider());
	//colliders.back()->SetTag("SphereCollider2");
}

CollisionScene::~CollisionScene()
{
	for (Collider* collider : colliders)
		delete collider;
}

void CollisionScene::Update()
{
	Ray ray = CAM->ScreenPointToRay(mousePos);
	RayHit hitInfo;


	//if (colliders[0]->IsRayCollision(ray, &hitInfo))
	//{
	//	colliders[0]->SetColor(1, 0, 0);
	//
	//	colliders[1]->SetLocalPosition(hitInfo.position + hitInfo.normal);
	//}
	//else
	//{
	//	colliders[0]->SetColor(0, 1, 0);
	//}

	if (colliders[0]->IsCollision(colliders[1]))
	{
		colliders[0]->SetColor(1, 0, 0);
		colliders[1]->SetColor(1, 0, 0);
	}
	else
	{
		colliders[0]->SetColor(0, 1, 0);
		colliders[1]->SetColor(0, 1, 0);
	}

	for (Collider* collider : colliders)
		collider->UpdateWorld();	
}

void CollisionScene::PreRender()
{
}

void CollisionScene::Render()
{
	for (Collider* collider : colliders)
		collider->Render();
}

void CollisionScene::PostRender()
{
}

void CollisionScene::GUIRender()
{
	for (Collider* collider : colliders)
		collider->Edit();
}
