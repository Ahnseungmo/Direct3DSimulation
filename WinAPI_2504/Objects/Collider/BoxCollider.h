#pragma once

class BoxCollider : public Collider
{
public:
	struct ObbDesc
	{
		Vector3 center;
		Vector3 axis[3];
		Vector3 halfSize;
	};

public:
	BoxCollider(Vector3 size = Vector3(1, 1, 1));
	~BoxCollider();

	void Edit();

	bool IsRayCollision(const Ray& ray, RayHit* hitInfo) override;
	bool IsBoxCollision(BoxCollider* collider) override;
	bool IsSphereCollision(SphereCollider* collider) override;
	bool IsCapsuleCollision(CapsuleCollider* collider) override;
	void FitSize(vector<ModelMesh*> meshes);
	void GetOBB(ObbDesc& desc);
	
	Vector3 GetSize() { return size; }
private:
	bool IsSeperateAxis(Vector3 D, Vector3 axis, ObbDesc box1, ObbDesc box2);

	void UpdateMesh();

	void SaveBox();
	void LoadBox();

	
private:
	void MakeMesh() override;

private:
	Vector3 size;
};
