#pragma once

class Water : public GameObject
{
public:
    Water(wstring normalFile, float width = 100.0f, float height = 100.0f);
    ~Water();

    void Update();

    void Render();
    void GUIRender();

    void SetReflection();
    void SetRefraction();

private:
    void MakeMesh();

private:
	Mesh<VertexUV>* mesh;
    Vector2 size;

    Reflection* reflection;
    Refraction* refraction;
};