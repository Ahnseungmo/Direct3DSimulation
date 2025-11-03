#include "Framework.h"
#include "Grid.h"

Grid::Grid() : GameObject(L"Basic/Grid.hlsl")
{
	mesh = new Mesh<VertexColor>();
	MakeMesh();
	mesh->CreateMesh();
}

Grid::~Grid()
{
	delete mesh;
}

void Grid::Render()
{
	if (!isActive) return;

	SetRender();
	mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void Grid::MakeMesh()
{
	vector<VertexColor>& vertices = mesh->GetVertices();
	
	//X Axis
	vertices.emplace_back(-MAX_VALUE, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	vertices.emplace_back(+MAX_VALUE, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);

	//Y Axis
	vertices.emplace_back(0.0f, -MAX_VALUE, 0.0f, 0.0f, 1.0f, 1.0f);
	vertices.emplace_back(0.0f, +MAX_VALUE, 0.0f, 0.0f, 1.0f, 1.0f);

	//Z Axis
	vertices.emplace_back(0.0f, 0.0f, -MAX_VALUE, 0.0f, 0.0f, 1.0f);
	vertices.emplace_back(0.0f, 0.0f, +MAX_VALUE, 0.0f, 0.0f, 1.0f);

	for (float i = -MAX_VALUE; i <= MAX_VALUE; i += 1.0f)
	{
		if (i == 0.0f) continue;

		//Parallel to Z Axis
		vertices.emplace_back(i, 0.0f, -MAX_VALUE, 0.5f, 0.5f, 0.5f);
		vertices.emplace_back(i, 0.0f, +MAX_VALUE, 0.5f, 0.5f, 0.5f);
		//Parallel to X Axis
		vertices.emplace_back(-MAX_VALUE, 0.0f, i, 0.5f, 0.5f, 0.5f);
		vertices.emplace_back(+MAX_VALUE, 0.0f, i, 0.5f, 0.5f, 0.5f);
	}
}
