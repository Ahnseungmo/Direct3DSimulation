#pragma once

class SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager();
	~SceneManager();

	void Update();
	void PreRender();
	void Render();
	void PostRender();
	void GUIRender();

	void AddScene(string key, Scene* scene);
	void ChangeScene(string key);

private:
	Scene* currentScene;
	
	unordered_map<string, Scene*> scenes;
};