#pragma once

class TerrainEditorScene : public Scene
{
public:
	TerrainEditorScene();
	~TerrainEditorScene();	

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	TerrainEditor* terrainEditor;
};