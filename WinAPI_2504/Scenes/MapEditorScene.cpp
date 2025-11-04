#include "Framework.h"
#include "MapEditorScene.h"

MapEditorScene::MapEditorScene()
{
	grid = new Grid();
}

MapEditorScene::~MapEditorScene()
{
	delete grid;
}

void MapEditorScene::Update()
{

}

void MapEditorScene::PreRender()
{
}

void MapEditorScene::Render()
{
	grid->Render();

}

void MapEditorScene::PostRender()
{
}

void MapEditorScene::GUIRender()
{
	

}
