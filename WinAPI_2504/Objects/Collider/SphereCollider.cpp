#include "Framework.h"

SphereCollider::SphereCollider(float radius, UINT stackCount, UINT sliceCount)
	: radius(radius), stackCount(stackCount), sliceCount(sliceCount)
{
    type = Type::Sphere;

    MakeMesh();
    mesh->CreateMesh();
}

bool SphereCollider::IsRayCollision(const Ray& ray, RayHit* hitInfo)
{
	UpdateWorld();

	Vector3 P = ray.origin;
	Vector3 D = ray.direction;

	Vector3 C = GetGlobalPosition();
	Vector3 A = P - C;
		
	float b = Vector3::Dot(A, D);
	float c = Vector3::Dot(A, A) - Radius() * Radius();

	if (b * b >= c)
	{
		if (hitInfo)
		{
			hitInfo->isHit = true;
			hitInfo->distance = -b - sqrt(b * b - c);
			hitInfo->position = P + D * hitInfo->distance;
			hitInfo->normal = (hitInfo->position - C).GetNormalized();
		}

		return true;
	}

    return false;
}

bool SphereCollider::IsBoxCollision(BoxCollider* collider)
{
    return false;
}

bool SphereCollider::IsSphereCollision(SphereCollider* collider)
{
    return false;
}

bool SphereCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
    return false;
}

void SphereCollider::MakeMesh()
{
	float thetaStep = XM_2PI / sliceCount;
	float phiStep = XM_PI / stackCount;

	vector<Vertex>& vertices = mesh->GetVertices();
	vertices.reserve((stackCount + 1) * (sliceCount + 1));

	for (UINT i = 0; i <= stackCount; i++)
	{
		float phi = i * phiStep;

		for (UINT j = 0; j <= sliceCount; j++)
		{
			float theta = j * thetaStep;

			float x = cos(theta) * sin(phi) * radius;
			float y = cos(phi) * radius;
			float z = sin(theta) * sin(phi) * radius;
			
			vertices.emplace_back(x, y, z);
		}
	}

	vector<UINT>& indices = mesh->GetIndices();	
	indices.reserve(stackCount * sliceCount * 4);

	for (UINT i = 0; i < stackCount; i++)
	{
		for (UINT j = 0; j < sliceCount; j++)
		{
			indices.push_back(i * (sliceCount + 1) + j);//0
			indices.push_back((i + 1) * (sliceCount + 1) + j);//1

			indices.push_back(i * (sliceCount + 1) + j);//0
			indices.push_back(i * (sliceCount + 1) + j + 1);//2
		}
	}
}
