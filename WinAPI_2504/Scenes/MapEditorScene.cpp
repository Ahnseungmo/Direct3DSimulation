#include "Framework.h"
#include "MapEditorScene.h"

MapEditorScene::MapEditorScene()
{
	grid = new Grid();
	InteriorManager::Get()->AddMeshType("SM_Barrel_2", 10);

	for (int i = 0; i < 3; i++)
	{
		InteriorObject* barrel = InteriorManager::Get()->Add("SM_Barrel_2");
		barrel->SetLocalPosition({ (float)i, (float)i, 0.0f });
	}
}

MapEditorScene::~MapEditorScene()
{
	delete grid;
}

void MapEditorScene::Update()
{
	InteriorManager::Get()->Update();

}

void MapEditorScene::PreRender()
{
}

void MapEditorScene::Render()
{
	grid->Render();

	InteriorManager::Get()->Render();
}

void MapEditorScene::PostRender()
{
}

void MapEditorScene::GUIRender()
{
	

}
