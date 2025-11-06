#include "Framework.h"
#include "MapEditorScene.h"


MapEditorScene::MapEditorScene()
{
	grid = new Grid();
	InteriorManager::Get()->AddMeshType("SM_Barrel_2", 200);
	InteriorManager::Get()->AddMeshType("SM_Floor", 200);
	InteriorManager::Get()->AddMeshType("SM_Basket", 200);
	InteriorManager::Get()->AddMeshType("SM_Table_1", 200);

	for (int i = 0; i < 100; i++)
	{
		InteriorObject* barrel = InteriorManager::Get()->Add("SM_Barrel_2", {(float)i,0,0});
		InteriorObject* floor = InteriorManager::Get()->Add("SM_Floor", { (float)i*4,0,0 });
		InteriorObject* basket = InteriorManager::Get()->Add("SM_Basket", { (float)i,0,0 });
		InteriorObject* table = InteriorManager::Get()->Add("SM_Table_1", { (float)i,0,0 });

		if (i == 0) barrel->SetLocalPosition(0,1,0);
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

	InteriorManager::Get()->RenderThumbnails();

	
	ID3D11RenderTargetView* rtv = Device::Get()->GetRTV();
	DC->OMSetRenderTargets(1, &rtv, Device::Get()->GetDSV());
	Environment::Get()->SetRender();

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
	InteriorManager::Get()->Edit();

	ImGui::Begin("Interior Objects");
	auto& models = InteriorManager::Get()->GetModelInstancing();
	for (auto const& pair : models)
	{
		ID3D11ShaderResourceView* textureID = InteriorManager::Get()->GetThumbnailSRV(pair.first);
		if (textureID)
		{

			if (ImGui::ImageButton(pair.first.c_str(), (ImTextureID)textureID, { 100, 100 }))
			{
			}
		}
	}
	ImGui::End();
}
