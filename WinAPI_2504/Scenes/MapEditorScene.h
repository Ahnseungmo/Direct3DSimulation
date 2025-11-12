#pragma once

class MapEditorScene : public Scene
{
public:
	MapEditorScene();
	~MapEditorScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Grid* grid;

	Room* room;
	Room* room2;
};