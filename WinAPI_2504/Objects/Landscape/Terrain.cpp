#include "Framework.h"

Terrain::Terrain() : Plane(L"Resources/Textures/HeightMaps/HeightMap.png")
{
	material->SetShader(L"Landscape/Terrain.hlsl");
	material->SetDiffuseMap(L"Resources/Textures/Landscape/Dirt2.png");

	alphaMap = Texture::Add(L"Resources/Textures/HeightMaps/AlphaMap.png");
	secondMap = Texture::Add(L"Resources/Textures/Landscape/Dirt.png");
}

Terrain::~Terrain()
{
}

void Terrain::Render()
{
	alphaMap->PSSet(10);
	secondMap->PSSet(11);

	Plane::Render();
}

float Terrain::GetHeight(const Vector3& pos)
{
	int x = (int)pos.x;
	int z = (int)pos.z;

	UINT width = size.x;
	UINT height = size.y;

	if (x < 0 || x >= width - 1) return 0.0f;
	if (z < 0 || z >= height - 1) return 0.0f;

	UINT index[4];
	index[0] = width * z + x;
	index[2] = width * z + x + 1;
	index[1] = width * (z + 1) + x;
	index[3] = width * (z + 1) + x + 1;

	vector<VertexType>& vertices = mesh->GetVertices();

	Vector3 p[4];
	for (UINT i = 0; i < 4; i++)
		p[i] = vertices[index[i]].pos;

	float u = pos.x - p[0].x;
	float v = pos.z - p[0].z;

	Vector3 result;

	if (u + v <= 1.0)
	{
		result = ((p[2] - p[0]) * u + (p[1] - p[0]) * v) + p[0];
		return result.y;
	}
	else
	{
		u = 1.0f - u;
		v = 1.0f - v;

		result = ((p[1] - p[3]) * u + (p[2] - p[3]) * v) + p[3];
		return result.y;
	}
}
