#pragma once

struct Ray
{
	Vector3 origin;
	Vector3 direction;

	Ray() {};
	Ray(Vector3 origin, Vector3 direction)
		: origin(origin), direction(direction.GetNormalized())
	{
	}
};

struct RayHit
{
	bool isHit = false;
	float distance = FLT_MAX;
	Vector3 position = Vector3();
	Vector3 normal = Vector3();
};

class Collider : public GameObject
{
protected:
	enum class Type
	{
		Box, Sphere, Capsule
	};

public:
	Collider();
	~Collider() = default;

	bool IsCollision(Collider* collider, Vector2* overlap = nullptr);

	virtual bool IsRayCollision(const Ray& ray, RayHit* hitInfo = nullptr) = 0;
	virtual bool IsBoxCollision(class BoxCollider* collider) = 0;
	virtual bool IsSphereCollision(class SphereCollider* collider) = 0;
	virtual bool IsCapsuleCollision(class CapsuleCollider* collider) = 0;
	void FitSize(vector<ModelMesh*> meshes);
	void Render() override;

private:
	virtual void MakeMesh() = 0;

public:
	static void SwitchDraw() { isDraw = !isDraw; }

protected:
	Type type;

	Mesh<Vertex>* mesh;

private:
	static bool isDraw;
};