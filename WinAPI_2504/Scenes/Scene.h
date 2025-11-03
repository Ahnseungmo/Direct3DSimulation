#pragma once

//Interface
class Scene
{
public:
	//Scene() {}
	Scene() = default;	
	~Scene() = default;
		
	virtual void Update() = 0;
	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;
	virtual void GUIRender() = 0;

	virtual void Start() {}
	virtual void End() {}
};