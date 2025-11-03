#include "Framework.h"

Steve::Steve()
{
	localPosition.y = 5.0f;
}

Steve::~Steve()
{
}

void Steve::Update()
{
	Move();
	Jump();
	Gravity();

	localPosition += velocity * DELTA;
	SphereCollider::UpdateWorld();
}

void Steve::Render()
{
	SphereCollider::Render();
}

void Steve::Move()
{
	Vector3 direction;

	if (Input::Get()->IsKeyPress('W'))
		direction += GetForward();
	if (Input::Get()->IsKeyPress('S'))
		direction -= GetForward();
	if (Input::Get()->IsKeyPress('A'))
		direction -= GetRight();
	if (Input::Get()->IsKeyPress('D'))
		direction += GetRight();

	Vector3 temp = direction.GetNormalized() * MOVE_SPEED;
	velocity.x = temp.x;
	velocity.z = temp.z;
}

void Steve::Jump()
{
	if (Input::Get()->IsKeyDown(VK_SPACE))
	{
		velocity.y = JUMP_POWER;
	}
		
	velocity.y += GRAVITY * DELTA;

	Ray ray = Ray(GetGlobalPosition(), Vector3::Down());
	RayHit hitInfo;

	//float floorHeight = BlockManager::Get()->GetFloorHeight(ray, &hitInfo);
	float floorHeight = 0.0f;

	if (velocity.y < 0 && GetGlobalPosition().y < floorHeight + Radius())
	{
		localPosition.y = floorHeight + Radius();		
		velocity.y = 0.0f;
	}
}

void Steve::Gravity()
{
}
