#include "Framework.h"

GameObject::GameObject()
{
	material = new Material();

    worldBuffer = new MatrixBuffer();
    
}

GameObject::GameObject(wstring shaderFile)
{
    material = new Material(shaderFile);

    worldBuffer = new MatrixBuffer();
}

GameObject::~GameObject()
{
    delete worldBuffer;
	delete material;
}

void GameObject::SetRender()
{
    worldBuffer->Set(world);
    worldBuffer->SetVS(0);

    material->Set();
}

void GameObject::SetWorld()
{
    worldBuffer->Set(world);
    worldBuffer->SetVS(0);
}

void GameObject::SetColor(float r, float g, float b, float a)
{
	material->SetColor(r, g, b, a);
}

void GameObject::MakeNormal(Mesh<VertexUVNormal>* mesh)
{
	vector<VertexUVNormal>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();

	for (UINT i = 0; i < indices.size(); i += 3)
	{
		UINT index0 = indices[i + 0];
		UINT index1 = indices[i + 1];
		UINT index2 = indices[i + 2];

		Vector3 p0 = vertices[index0].pos;
		Vector3 p1 = vertices[index1].pos;
		Vector3 p2 = vertices[index2].pos;

		Vector3 e0 = p1 - p0;
		Vector3 e1 = p2 - p0;
		
		Vector3 normal = Vector3::Cross(e0, e1);

		vertices[index0].normal += normal;
		vertices[index1].normal += normal;
		vertices[index2].normal += normal;
	}
}

void GameObject::MakeNormal(Mesh<VertexUVNormalTangent>* mesh)
{
	vector<VertexUVNormalTangent>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();

	for (UINT i = 0; i < indices.size(); i += 3)
	{
		UINT index0 = indices[i + 0];
		UINT index1 = indices[i + 1];
		UINT index2 = indices[i + 2];

		Vector3 p0 = vertices[index0].pos;
		Vector3 p1 = vertices[index1].pos;
		Vector3 p2 = vertices[index2].pos;

		Vector3 e0 = p1 - p0;
		Vector3 e1 = p2 - p0;

		Vector3 normal = Vector3::Cross(e0, e1);

		vertices[index0].normal += normal;
		vertices[index1].normal += normal;
		vertices[index2].normal += normal;
	}
}

void GameObject::MakeTangent(Mesh<VertexUVNormalTangent>* mesh)
{
	vector<VertexUVNormalTangent>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();

	for (UINT i = 0; i < indices.size(); i += 3)
	{
		UINT index0 = indices[i + 0];
		UINT index1 = indices[i + 1];
		UINT index2 = indices[i + 2];

		Vector3 p0 = vertices[index0].pos;
		Vector3 p1 = vertices[index1].pos;
		Vector3 p2 = vertices[index2].pos;

		Vector2 uv0 = vertices[index0].uv;
		Vector2 uv1 = vertices[index1].uv;
		Vector2 uv2 = vertices[index2].uv;

		Vector3 e0 = p1 - p0;
		Vector3 e1 = p2 - p0;

		float u0 = uv1.x - uv0.x;
		float u1 = uv2.x - uv0.x;
		float v0 = uv1.y - uv0.y;
		float v1 = uv2.y - uv0.y;

		float d = 1.0f / (u0 * v1 - u1 * v0);
		Vector3 tangent = (e0 * v1 - e1 * v0) * d;

		vertices[index0].tangent += tangent;
		vertices[index1].tangent += tangent;
		vertices[index2].tangent += tangent;
	}
}
