#pragma once

class TerrainScene : public Scene
{
private:
	const UINT SIZE = 1000;

public:
	TerrainScene();
	~TerrainScene();	

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Terrain* terrain;
	Skybox* skybox;

	//vector<Quad*> trees;
	Material* material;
	VertexBuffer* vertexBuffer;
	GeometryShader* geometryShader;

	vector<VertexUV> vertices;
};