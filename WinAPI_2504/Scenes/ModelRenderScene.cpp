#include "Framework.h"
#include "ModelRenderScene.h"

ModelRenderScene::ModelRenderScene()
{
	model = new Model("SM_Wardrobe_1");
	collider = new BoxCollider();
	model->SetParent(collider);


	model2 = new Model("Naruto");
	grid = new Grid();
}

ModelRenderScene::~ModelRenderScene()
{
	delete model;
	delete model2;
	delete grid;
	delete collider;

}

void ModelRenderScene::Update()
{

	if (Input::Get()->IsKeyDown(VK_F4)) {
		collider->FitSize(model->GetMeshes());
		collider->SetLocalScale(model->GetLocalScale());
		model->SetLocalPosition(0, 0, 0);
		model->UpdateWorld();

		//		model->SetLocalPosition();
	}


	grid->UpdateWorld();
	model->UpdateWorld();
	model2->UpdateWorld();

	collider->UpdateWorld();
}

void ModelRenderScene::PreRender()
{
}

void ModelRenderScene::Render()
{
	grid->Render();
	model->Render();
	model2->Render();
	collider->Render();
}

void ModelRenderScene::PostRender()
{
}

void ModelRenderScene::GUIRender()
{
	model->Edit();
	model2->Edit();
	collider->Edit();
}
