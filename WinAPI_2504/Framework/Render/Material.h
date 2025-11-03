#pragma once

class Material
{
private:
	enum MapType
	{
		Diffuse, Specular, Normal
	};

public:
	Material(wstring shaderFile = L"Light/Light.hlsl");
	~Material();	

	void Edit();

	void Save(string path);
	void Load(string path);

	void SetColor(float r, float g, float b, float a = 1.0f);

	void SetShader(wstring file);
	void SetVertexShader(wstring file);
	void SetPixelShader(wstring file);

	void Set();
	
	void SetDiffuseMap(wstring file);
	void SetDiffuseMap(Texture* texture) { diffuseMap = texture; }

	void SetSpecularMap(wstring file);
	void SetSpecularMap(Texture* texture) { specularMap = texture; }

	void SetNormalMap(wstring file);
	void SetNormalMap(Texture* texture) { normalMap = texture; }

	Texture* GetDiffuseMap() { return diffuseMap; }

	MaterialBuffer* GetBuffer() { return materialBuffer; }
	MaterialBuffer::Data* GetData() { return materialBuffer->GetData(); }

	string GetName() { return name; }
	void SetName(string name) { this->name = name; }

private:
	void SelectMap(string name, MapType mapType);	
	void UnselectMap(MapType mapType);

	void SaveDialog();
	void LoadDialog();

private:
	string name;
	string editName;
	string file;
	string projectPath;

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	MaterialBuffer* materialBuffer;

	Texture* diffuseMap = nullptr;
	Texture* specularMap = nullptr;
	Texture* normalMap = nullptr;
};