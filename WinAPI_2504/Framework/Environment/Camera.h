#pragma once

class Camera : public Transform
{
public:
	Camera();
	~Camera();

	void Update();
	void Edit();

	void SetView(UINT slot = 1);

	void SetTarget(Transform* target) { this->target = target; }

	class Ray ScreenPointToRay(Vector3 screenPos);	
	Vector3 WorldToScreenPoint(Vector3 worldPos);

	bool ContainPoint(Vector3 point);
	bool ContainSphere(Vector3 center, float radius);

private:
	void FreeMode();
	void FollowMode();

	void FrusutmUpdate();

private:
	float moveSpeed = 10.0f;
	float rotSpeed = 10.0f;
	Vector3 prevMousePos;

	Matrix view;
	Matrix projection;
	ViewBuffer* viewBuffer;	

	Transform* target = nullptr;	

	float distance = 20.0f;
	float height = 20.0f;
	float moveDamping = 5.0f;
	float rotDamping = 1.0f;
	float destRot = 0.0f;

	Vector3 destPos;
	Vector3 focusOffset;

	Matrix rotMatrix;

	bool isLookAtTargetX = true;
	bool isLookAtTargetY = true;

	Vector3 planes[6];
	float a, b, c, d;
};