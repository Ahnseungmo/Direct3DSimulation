#pragma once

class Cube : public GameObject
{
private:
	typedef VertexUVNormal VertexType;

public:
	Cube(Vector3 size = Vector3(1, 1, 1));	
	~Cube();

	void Render();

	Vector3 GetSize() const { return size; }

private:
	void MakeMesh();

private:
	Vector3 size;

	Mesh<VertexType>* mesh;
};