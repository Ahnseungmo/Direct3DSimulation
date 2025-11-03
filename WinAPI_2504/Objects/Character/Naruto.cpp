#include "Framework.h"

Naruto::Naruto()
{
	tag = "NarutoCapsule";

	model = new ModelAnimator("Naruto");
	model->ReadClip("Idle");
	model->ReadClip("Run");
	model->ReadClip("Attack");
	model->CreateTexture();

	model->SetParent(this);
	model->Load();
	
	model->GetClip(Attack)->SetEvent(bind(&Naruto::EndAttack, this), 0.9f);

	sword = new Sword();

	rightHand = new Transform();

	sword->SetParent(rightHand);

	//CAM->SetTarget(this);
}

Naruto::~Naruto()
{
	delete model;
	delete sword;
	delete rightHand;
}

void Naruto::Update()
{
	rightHand->SetWorld(model->GetTransformByNode(36));

	Move();
	StartAttack();

	model->Update();
	UpdateWorld();

	sword->Update();
}

void Naruto::Render()
{
	model->Render();
	Collider::Render();

	sword->Render();
}

void Naruto::Edit()
{
	Transform::Edit();
	model->Edit();

	sword->Edit();	
}

void Naruto::Move()
{
	if (state == Attack)
		return;

	moveDir = Vector3::Zero();

	if (Input::Get()->IsKeyPress('W'))
	{
		//moveDir += Vector3::Forward();
		moveDir += GetForward();
	}
	else if (Input::Get()->IsKeyPress('S'))
	{
		moveDir += Vector3::Back();
	}

	if (Input::Get()->IsKeyPress('A'))
	{
		moveDir += Vector3::Left();
	}
	else if (Input::Get()->IsKeyPress('D'))
	{
		moveDir += Vector3::Right();
	}

	Vector3 delta = mousePos - prevMousePos;
	prevMousePos = mousePos;

	localRotation.y += delta.x * rotateSpeed * DELTA;
	//CAM->Rotate(Vector3::Right(), -delta.y * rotateSpeed * DELTA);

	if (moveDir.Length() > 0.0f)
	{
		moveDir.Normalize();	
		Translate(moveDir * moveSpeed * DELTA);
		//Rotate();

		PlayClip(Run);
	}
	else
	{
		PlayClip(Idle);
	}
}

void Naruto::Rotate()
{
	Vector3 lookDir = moveDir;
	lookDir.y = 0.0f;
	lookDir.Normalize();
	Vector3 forward = GetForward();
	forward.y = 0.0f;
	forward.Normalize();
	float dot = Vector3::Dot(forward, lookDir);
	dot = GameMath::Clamp(dot, -1.0f, 1.0f);
	float angle = acosf(dot);
	Vector3 cross = Vector3::Cross(forward, lookDir);

	if (cross.y < 0.0f)
		angle = -angle;

	Transform::Rotate(Vector3::Up(), angle * rotateSpeed * DELTA);
}

void Naruto::StartAttack()
{
	if (Input::Get()->IsKeyDown(VK_LBUTTON))
		PlayClip(Attack);
}

void Naruto::EndAttack()
{
	PlayClip(Idle);
}

void Naruto::PlayClip(State state)
{
	if (this->state == state)
		return;

	this->state = state;
	model->PlayClip((int)state);
}
