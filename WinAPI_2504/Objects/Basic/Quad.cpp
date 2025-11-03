#include "Framework.h"

Quad::Quad(Vector2 size) : size(size), GameObject(L"Basic/Texture.hlsl")
{
	tag = "Quad";

	mesh = new Mesh<VertexType>();
	MakeMesh();
	mesh->CreateMesh();
}

Quad::Quad(wstring textureFile, Vector2 startUV, Vector2 endUV)
	: startUV(startUV), endUV(endUV)
{
	material->SetDiffuseMap(textureFile);

	size = material->GetDiffuseMap()->GetSize() * (endUV - startUV);

	mesh = new Mesh<VertexType>();
	MakeMesh();
	mesh->CreateMesh();
}

Quad::~Quad()
{
	delete mesh;
}

void Quad::Render()
{
	if (!isActive) return;

	SetRender();
	mesh->Draw();
}

void Quad::RenderInstanced(UINT count)
{
	if (!isActive) return;

	SetRender();
	mesh->DrawInstanced(count);
}

void Quad::MakeMesh()
{
	Vector2 halfSize = size * 0.5f;

	vector<VertexType>& vertices = mesh->GetVertices();

	vertices.emplace_back(-halfSize.x, +halfSize.y, 0.0f, startUV.x, startUV.y);
	vertices.emplace_back(+halfSize.x, +halfSize.y, 0.0f, endUV.x, startUV.y);
	vertices.emplace_back(-halfSize.x, -halfSize.y, 0.0f, startUV.x, endUV.y);
	vertices.emplace_back(+halfSize.x, -halfSize.y, 0.0f, endUV.x, endUV.y);

	vector<UINT>& indices = mesh->GetIndices();

	indices = { 0, 1, 2, 2, 1, 3 };
}
