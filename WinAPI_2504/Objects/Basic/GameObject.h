#pragma once

class GameObject : public Transform
{
public:
	GameObject();
	GameObject(wstring shaderFile);
	~GameObject();	

	virtual void Render() {}
	virtual void SetRender();

	void SetWorld();
	
	Material* GetMaterial() { return material; }

	void SetColor(float r, float g, float b, float a = 1.0f);	

protected:
	void MakeNormal(Mesh<VertexUVNormal>* mesh);
	void MakeNormal(Mesh<VertexUVNormalTangent>* mesh);
	void MakeTangent(Mesh<VertexUVNormalTangent>* mesh);

protected:
	Material* material;
	MatrixBuffer* worldBuffer;	
};