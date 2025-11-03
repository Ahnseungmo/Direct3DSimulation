#pragma once

class Grid : public GameObject
{
private:
	float MAX_VALUE = 100.0f;

public:
	Grid();
	~Grid();

	void Render();

private:
	void MakeMesh();

private:
	Mesh<VertexColor>* mesh;
};