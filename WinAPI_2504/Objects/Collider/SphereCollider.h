#pragma once

class SphereCollider : public Collider
{
public:
	SphereCollider(float radius = 1.0f, UINT stackCount = 8, UINT sliceCount = 16);
	~SphereCollider() = default;

	bool IsRayCollision(const Ray& ray, RayHit* hitInfo) override;
	bool IsBoxCollision(BoxCollider* collider) override;
	bool IsSphereCollision(SphereCollider* collider) override;
	bool IsCapsuleCollision(CapsuleCollider* collider) override;

	float Radius() { return radius * max(GetGlobalScale().x, max(GetGlobalScale().y, GetGlobalScale().z)); }

private:
	void MakeMesh() override;

private:
	float radius;
	UINT stackCount;
	UINT sliceCount;
};