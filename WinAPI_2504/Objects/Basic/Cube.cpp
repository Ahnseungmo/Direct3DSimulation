#include "Framework.h"

Cube::Cube(Vector3 size) : size(size)
{
	tag = "Cube";

	mesh = new Mesh<VertexType>();
	MakeMesh();
	MakeNormal(mesh);
	mesh->CreateMesh();
}

Cube::~Cube()
{
	delete mesh;
}

void Cube::Render()
{
	if (!isActive) return;

	SetRender();
	mesh->Draw();
}

void Cube::MakeMesh()
{
	Vector3 halfSize = size * 0.5f;
	vector<VertexType>& vertices = mesh->GetVertices();
	//Front
	vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z, 0, 0);
	vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z, 1, 0);
	vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z, 0, 1);
	vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z, 1, 1);
	//Back
	vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, 0, 0);
	vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, 1, 0);
	vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, 0, 1);
	vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, 1, 1);
	//Left
	vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, 0, 0);
	vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z, 1, 0);
	vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, 0, 1);
	vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z, 1, 1);
	////Right
	vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z, 0, 0);
	vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, 1, 0);
	vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z, 0, 1);
	vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, 1, 1);
	//Up
	vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, 0, 0);
	vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, 1, 0);
	vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z, 0, 1);
	vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z, 1, 1);
	//Down
	vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z, 0, 0);
	vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z, 1, 0);
	vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, 0, 1);
	vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, 1, 1);

	vector<UINT>& indices = mesh->GetIndices();
	indices = {
		0, 1, 2, 2, 1, 3,
		4, 5, 6, 6, 5, 7,
		8, 9,10,10, 9,11,
	   12,13,14,14,13,15,
	   16,17,18,18,17,19,
	   20,21,22,22,21,23
	};
}
