#pragma once

class Sphere : public GameObject
{
private:
	typedef VertexUVNormalTangent VertexType;

public:
	Sphere(float radius = 1.0f, UINT stackCount = 8, UINT sliceCount = 16);
	~Sphere();

	void Render();

private:
	void MakeMesh();

private:
	float radius;
	UINT stackCount;
	UINT sliceCount;

	Mesh<VertexType>* mesh;
};