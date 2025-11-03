#include "Framework.h"
#include "WaterScene.h"

WaterScene::WaterScene()
{
	CreateObjects();

    //water = new Water(L"Resources/Textures/Landscape/Wave.dds");
    //reflection = new Reflection(plane);
	//refraction = new Refraction(L"Resources/Textures/Landscape/Wave.dds");
    
    //plane->GetMaterial()->SetShader(L"Landscape/Reflection.hlsl");
    //plane->GetMaterial()->SetShader(L"Landscape/Refraction.hlsl");
    plane->GetMaterial()->SetShader(L"Light/Shadow.hlsl");

    shadow = new Shadow();
}

WaterScene::~WaterScene()
{
    delete sphere;
    delete model;
    delete animator;
    delete skybox;
	//delete water;
	delete plane;
    delete shadow;

    //delete refraction;
}

void WaterScene::Update()
{
    //water->Update();
    //refraction->Update();

    sphere->UpdateWorld();
    model->UpdateWorld();
    animator->Update();

    //plane->UpdateWorld();
}

void WaterScene::PreRender()
{
    //refraction->SetRefraction();
    //water->SetRefraction();
    //
    //skybox->Render();
    //sphere->Render();
    //model->Render();
    //animator->Render();
    //
    //water->SetReflection();
    //
    //skybox->Render();
    //sphere->Render();
    //model->Render();
    //animator->Render();

    shadow->SetRenderTarget();
    sphere->GetMaterial()->SetShader(L"Light/DepthMap.hlsl");
    sphere->Render();
    model->SetShader(L"Light/DepthMap.hlsl");
    model->Render();
    animator->SetShader(L"Light/SkinDepthMap.hlsl");
    animator->Render();
}

void WaterScene::Render()
{
    //skybox->Render();

    sphere->GetMaterial()->SetShader(L"Light/Light.hlsl");
    sphere->Render();
    model->SetShader(L"Light/Light.hlsl");
    model->Render();
    animator->SetShader(L"Model/Model.hlsl");
    animator->Render();

    shadow->SetRender();
    plane->Render();
    //water->Render();

	//refraction->SetRender();
    //plane->Render();
}

void WaterScene::PostRender()
{
    //refraction->PostRender();
    shadow->PostRender();
}

void WaterScene::GUIRender()
{
    //refraction->GUIRender();

    //water->GUIRender();

    //sphere->Edit();
    //model->Edit();
    //animator->Edit();

    //plane->Edit();
}

void WaterScene::CreateObjects()
{
    sphere = new Sphere();
    sphere->SetTag("TestSphere");
    sphere->GetMaterial()->Load("Resources/Materials/Stone.mat");
    sphere->Load();

    model = new Model("Character");
    model->SetTag("TestModel");
    model->Load();

    animator = new ModelAnimator("Naruto");
    animator->SetTag("TestAnimator");
    animator->ReadClip("Idle");
    animator->CreateTexture();
    animator->Load();

    skybox = new Skybox(L"Resources/Textures/Landscape/Snow_ENV.dds");

    plane = new Plane();
}
