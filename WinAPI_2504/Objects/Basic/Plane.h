#pragma once

class Plane : public GameObject
{
protected:
	typedef VertexUVNormalTangent VertexType;

public:
	Plane(Vector2 size = Vector2(10, 10));
	Plane(wstring heightMapFile);
	~Plane();

	void Render();

	Vector2 GetSize() const { return size; }

private:
	void MakeMesh();
	void MakeMesh(Texture* heightMap);

protected:
	Vector2 size;

	Mesh<VertexType>* mesh;
};