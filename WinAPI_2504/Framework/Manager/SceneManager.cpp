#include "Framework.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
	for (pair<string, Scene*> scene : scenes)
	{
		delete scene.second;
	}
}

void SceneManager::Update()
{
	currentScene->Update();
}

void SceneManager::PreRender()
{
	currentScene->PreRender();
}

void SceneManager::Render()
{
	currentScene->Render();	
}

void SceneManager::PostRender()
{
	currentScene->PostRender();
}

void SceneManager::GUIRender()
{
	currentScene->GUIRender();
}

void SceneManager::AddScene(string key, Scene* scene)
{
	if (scenes.count(key) > 0)
		return;

	scenes[key] = scene;
}

void SceneManager::ChangeScene(string key)
{
	if (scenes.count(key) == 0)
		return;

	//if (currentScene != nullptr)
	if(currentScene)
	{
		currentScene->End();
	}

	currentScene = scenes[key];
	currentScene->Start();
}
