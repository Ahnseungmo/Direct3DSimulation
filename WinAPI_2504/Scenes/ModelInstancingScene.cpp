#include "Framework.h"
#include "ModelInstancingScene.h"

ModelInstancingScene::ModelInstancingScene()
{
	modelInstancing = new ModelAnimatorInstancing("Naruto");

	modelInstancing->ReadClip("Idle");
	modelInstancing->ReadClip("Run");
	modelInstancing->ReadClip("Attack");

	modelInstancing->CreateTexture();

	for (float z = -5.0f; z <= 5.0f; z++)
	{
		for (float x = -5.0f; x <= 5.0f; x++)
		{
			//Model* model = new Model("Naruto");
			//model->SetLocalPosition(x, 0.0f, z);
			////model->SetLocalScale(0.01f, 0.01f, 0.01f);
			//model->UpdateWorld();
			//models.push_back(model);

			Transform* transform = modelInstancing->Add();
			transform->SetLocalPosition(x, 0.0f, z);
			transform->SetLocalScale(0.01f, 0.01f, 0.01f);
		}
	}

	//modelInstancing->Update();
}

ModelInstancingScene::~ModelInstancingScene()
{
	//for (Model* model : models)
	//	delete model;
	delete modelInstancing;
}

void ModelInstancingScene::Update()
{
	modelInstancing->Update();
}

void ModelInstancingScene::PreRender()
{
}

void ModelInstancingScene::Render()
{
	//for (Model* model : models)
	//{
	//	if (CAM->ContainPoint(model->GetGlobalPosition()))
	//		model->Render();
	//}

	modelInstancing->Render();
}

void ModelInstancingScene::PostRender()
{
}

void ModelInstancingScene::GUIRender()
{
	ImGui::DragInt("Index", &instanceIndex);
	ImGui::DragInt("Clip", &clip);

	if (ImGui::Button("Play"))
		modelInstancing->PlayClip(instanceIndex, clip);

	modelInstancing->Edit();
}
