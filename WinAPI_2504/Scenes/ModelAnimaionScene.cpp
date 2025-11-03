#include "Framework.h"
#include "ModelAnimaionScene.h"

ModelAnimationScene::ModelAnimationScene()
{
	model = new ModelAnimator("Naruto");
	model->SetVertexShader(L"Model/Model.hlsl");

	//model->ReadClip("Attack", 1);
	model->ReadClip("Idle", 0);
	model->ReadClip("Run", 0);
	model->ReadClip("Attack", 0);
	model->CreateTexture();
}

ModelAnimationScene::~ModelAnimationScene()
{
	delete model;
}

void ModelAnimationScene::Update()
{
	if (Input::Get()->IsKeyDown('1'))
		model->PlayClip(0);
	if (Input::Get()->IsKeyDown('2'))
		model->PlayClip(1);
	if (Input::Get()->IsKeyDown('3'))
		model->PlayClip(2);

	model->Update();
}

void ModelAnimationScene::PreRender()
{
}

void ModelAnimationScene::Render()
{
	model->Render();
}

void ModelAnimationScene::PostRender()
{
}

void ModelAnimationScene::GUIRender()
{
	model->Edit();
}
