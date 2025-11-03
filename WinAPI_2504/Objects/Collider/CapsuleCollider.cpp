#include "Framework.h"

CapsuleCollider::CapsuleCollider(float radius, float height, UINT stackCount, UINT sliceCount)
    : radius(radius), height(height), stackCount(stackCount), sliceCount(sliceCount)
{
    type = Type::Capsule;

    MakeMesh();
    mesh->CreateMesh();
}

bool CapsuleCollider::IsRayCollision(const Ray& ray, RayHit* hitInfo)
{
    return false;
}

bool CapsuleCollider::IsBoxCollision(BoxCollider* collider)
{
    return false;
}

bool CapsuleCollider::IsSphereCollision(SphereCollider* collider)
{
    return false;
}

bool CapsuleCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
    return false;
}

void CapsuleCollider::MakeMesh()
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

			if (y > 0.0f)
				y += (height * 0.5f);
			else if (y < 0.0f)
				y -= (height * 0.5f);

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

void CapsuleCollider::UpdateMesh()
{
}
