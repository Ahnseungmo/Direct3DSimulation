#include "Framework.h"

#include "Scenes/TutorialScene.h"
#include "Scenes/CubeScene.h"
#include "Scenes/CollisionScene.h"
#include "Scenes/SphereScene.h"
#include "Scenes/BlockScene.h"
#include "Scenes/LightScene.h"
#include "Scenes/ModelExportScene.h"
#include "Scenes/ModelRenderScene.h"
#include "Scenes/ModelAnimaionScene.h"
#include "Scenes/CharacterScene.h"
#include "Scenes/ModelInstancingScene.h"
#include "Scenes/TerrainScene.h"
#include "Scenes/TerrainEditorScene.h"
#include "Scenes/WeatherScene.h"
#include "Scenes/ParticleEditorScene.h"
#include "Scenes/WaterScene.h"
#include "Scenes/ModularModelExportScene.h"
#include "Scenes/MapEditorScene.h"
GameManager::GameManager()
{
	Create();
		
	//SCENE->AddScene("Game", new TutorialScene());	
	//SCENE->AddScene("Game", new CubeScene());
	//SCENE->AddScene("Game", new CollisionScene());
	//SCENE->AddScene("Game", new SphereScene());
	//SCENE->AddScene("Game", new BlockScene());
	//SCENE->AddScene("Game", new LightScene());
	//SCENE->AddScene("Export", new ModelExportScene());
	//SCENE->AddScene("Game", new ModelRenderScene());
	//SCENE->AddScene("Game", new ModelAnimationScene());
	//SCENE->AddScene("Game", new MapEditorScene());
	//SCENE->AddScene("Game", new ModelInstancingScene());
	//SCENE->AddScene("Game", new TerrainScene());
	//SCENE->AddScene("Game", new TerrainEditorScene());
	//SCENE->AddScene("Game", new WeatherScene());
	//SCENE->AddScene("Game", new ParticleEditorScene());
	//SCENE->AddScene("Game", new WaterScene());
	//SCENE->AddScene("Game", new ModularModelExportScene());
	SCENE->AddScene("Game", new MapEditorScene());

	SCENE->ChangeScene("Game");
}

GameManager::~GameManager()
{
	Release();
}

void GameManager::Update()
{
	Timer::Get()->Update();
	Input::Get()->Update();
	Audio::Get()->Update();

	Environment::Get()->Update();

	SCENE->Update();

	InvalidateRect(hWnd, nullptr, false);
}

void GameManager::Render()
{
	SCENE->PreRender();	
	
	Device::Get()->Clear();	

	Environment::Get()->SetRender();
	SCENE->Render();

	Environment::Get()->SetPostRender();
	SCENE->PostRender();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
	ImGui::Text(fps.c_str());

	Environment::Get()->Edit();

	SCENE->GUIRender();	

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	Device::Get()->Present();
}

void GameManager::Create()
{
	Timer::Get();
	Input::Get();
	Audio::Get();

	Device::Get();

	SceneManager::Get();

	Environment::Get();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(DEVICE, DC);
}

void GameManager::Release()
{
	Timer::Delete();
	Input::Delete();

	Device::Delete();
	Shader::Delete();
	Texture::Delete();
	Audio::Delete();

	SceneManager::Delete();

	Environment::Delete();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}
