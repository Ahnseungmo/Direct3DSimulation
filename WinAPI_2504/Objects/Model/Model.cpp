#include "Framework.h"

Model::Model(string name) : name(name)
{
	tag = name;

	ReadMaterial();
	ReadMesh();

	worldBuffer = new MatrixBuffer();
}

Model::~Model()
{
	for (Material* material : materials)
		delete material;
	for (ModelMesh* mesh : meshes)
		delete mesh;

	delete worldBuffer;
}

void Model::Render()
{
	worldBuffer->Set(world);
	worldBuffer->SetVS(0);

	for (ModelMesh* mesh : meshes)
		mesh->Render();
}

void Model::Edit()
{
	Transform::Edit();

	for (Material* material : materials)
		material->Edit();
}

void Model::SetShader(wstring file)
{
	for (Material* material : materials)
		material->SetShader(file);
}

void Model::SetVertexShader(wstring file)
{
	for (Material* material : materials)
		material->SetVertexShader(file);
}

void Model::SetPixelShader(wstring file)
{
	for (Material* material : materials)
		material->SetPixelShader(file);
}

void Model::ReadMaterial()
{
	string file = "Resources/Models/Materials/" + name + "/" + name + ".mats";

	BinaryReader* reader = new BinaryReader(file);
	if (reader->IsFailed())
		assert(false);

	UINT size = reader->Int();

	materials.reserve(size);

	for (UINT i = 0; i < size; i++)
	{
		Material* material = new Material();
		material->Load(reader->String());

		materials.push_back(material);
	}

	delete reader;
}

void Model::ReadMesh()
{
	string file = "Resources/Models/Meshes/" + name + ".mesh";

	BinaryReader* reader = new BinaryReader(file);
	if (reader->IsFailed())
		assert(false);

	UINT size = reader->UInt();

	for (UINT i = 0; i < size; i++)
	{
		ModelMesh* mesh = new ModelMesh(reader->String());

		UINT materialIndex = reader->UInt();
		mesh->SetMaterial(materials[materialIndex], materialIndex);

		vector<ModelVertex> vertices;		
		UINT vertexCount = reader->UInt();
		vertices.resize(vertexCount);		
		reader->Byte(vertices.data(), sizeof(ModelVertex) * vertexCount);

		vector<UINT> indices;
		UINT indexCount = reader->UInt();
		indices.resize(indexCount);				
		reader->Byte(indices.data(), sizeof(UINT) * indexCount);

		mesh->CreateMesh(vertices.data(), vertexCount, indices.data(), indexCount);

		meshes.push_back(mesh);
	}

	size = reader->Data<UINT>();
	nodes.resize(size);
	for (NodeData& node : nodes)
	{
		node.index = reader->Data<int>();
		node.name = reader->String();
		node.parent = reader->Data<int>();
		node.transform = reader->Data<Matrix>();
	}

	size = reader->Data<UINT>();
	bones.resize(size);
	for (BoneData& bone : bones)
	{
		bone.index = reader->Data<int>();
		bone.name = reader->String();
		bone.offset = reader->Data<Matrix>();

		boneMap[bone.name] = bone.index;
	}

	delete reader;
}
