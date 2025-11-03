#pragma once

class CollisionScene : public Scene
{
public:
	CollisionScene();
	~CollisionScene();	

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	vector<Collider*> colliders;
};