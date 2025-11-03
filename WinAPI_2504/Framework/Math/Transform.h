#pragma once

class Transform
{
public:
	Transform();
	~Transform();

	void Translate(const Vector3& velocity)
	{
		localPosition += velocity;
	}

	void Rotate(const Vector3& axis, float angle)
	{
		localRotation += axis * angle;
	}

	void UpdateWorld();

	void Edit();

	void Save();
	void Load();

	void SetActive(bool isActive) { this->isActive = isActive; }
	bool IsActive();
	bool IsActiveSelf() { return isActive; }

	Vector3 GetRight() { return right.GetNormalized(); }
	Vector3 GetUp() { return up.GetNormalized(); }
	Vector3 GetForward() { return forward.GetNormalized(); }

	Vector3 GetGlobalPosition() { return globalPosition; }
	Vector3 GetGlobalScale() { return globalScale; }

	Vector3 GetLocalPosition() { return localPosition; }
	void SetLocalPosition(float x, float y, float z) { localPosition = { x, y, z}; }
	void SetLocalPosition(Vector3 pos) { this->localPosition = pos; }

	Vector3 GetLocalRotation() { return localRotation; }
	void SetLocalRotation(float x, float y, float z) { localRotation = { x, y, z }; }
	void SetLocalRotation(Vector3 rotation) { this->localRotation = rotation; }

	Vector3 GetLocalScale() { return localScale; }
	void SetLocalScale(float x, float y, float z) { localScale = { x, y, z}; }
	void SetLocalScale(Vector3 scale) { this->localScale = scale; }

	void SetPivot(float x, float y, float z) { pivot = { x, y, z}; }

	Matrix GetWorld() { return world; }
	void SetWorld(const Matrix& matrix) { world = matrix; }

	void SetParent(Transform* transform) { parent = transform; }	

	string GetTag() { return tag; }
	void SetTag(string tag) { this->tag = tag; }

protected:
	string tag;

	bool isActive = true;

	Vector3 localPosition;
	Vector3 localRotation;
	Vector3 localScale = { 1, 1, 1};	
	Vector3 pivot;

	Vector3 right = { 1, 0, 0 };
	Vector3 up = { 0, 1, 0};
	Vector3 forward = { 1, 0, 0 };

	Matrix world;
	Float4x4 matWorld;

private:
	Matrix S, R, T;
	Matrix P, IP;

	Vector3 globalPosition;
	Vector3 globalScale = { 1, 1, 1};	

	Transform* parent = nullptr;
};