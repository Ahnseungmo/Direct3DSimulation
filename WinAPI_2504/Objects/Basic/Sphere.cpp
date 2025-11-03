#include "Framework.h"

Sphere::Sphere(float radius, UINT stackCount, UINT sliceCount)
	: radius(radius), stackCount(stackCount), sliceCount(sliceCount)
{
	tag = "Sphere";

	mesh = new Mesh<VertexType>();
	MakeMesh();
	MakeTangent(mesh);
	mesh->CreateMesh();
}

Sphere::~Sphere()
{
	delete mesh;
}

void Sphere::Render()
{
	if (!isActive) return;

	SetRender();
	mesh->Draw();
}

void Sphere::MakeMesh()
{
	float thetaStep = XM_2PI / sliceCount;
	float phiStep = XM_PI / stackCount;

	vector<VertexType>& vertices = mesh->GetVertices();
	vertices.reserve((stackCount + 1) * (sliceCount + 1));

	for (UINT i = 0; i <= stackCount; i++)
	{
		float phi = i * phiStep;

		for (UINT j = 0; j <= sliceCount; j++)
		{
			float theta = j * thetaStep;

			VertexType vertex;

			vertex.normal.x = cos(theta) * sin(phi);
			vertex.normal.y = cos(phi);
			vertex.normal.z = sin(theta) * sin(phi);

			vertex.pos = Vector3(vertex.normal) * radius;

			vertex.uv.x = (float)j / sliceCount;
			vertex.uv.y = (float)i / stackCount;

			vertices.emplace_back(vertex);
		}
	}

	vector<UINT>& indices = mesh->GetIndices();
	indices.reserve(stackCount * sliceCount * 6);

	for (UINT i = 0; i < stackCount; i++)
	{
		for (UINT j = 0; j < sliceCount; j++)
		{
			indices.push_back(i * (sliceCount + 1) + j);//0			
			indices.push_back((i + 1) * (sliceCount + 1) + j + 1);//2
			indices.push_back((i + 1) * (sliceCount + 1) + j);//1

			indices.push_back(i * (sliceCount + 1) + j);//0			
			indices.push_back(i * (sliceCount + 1) + j + 1);//3
			indices.push_back((i + 1) * (sliceCount + 1) + j + 1);//2
		}
	}
}
