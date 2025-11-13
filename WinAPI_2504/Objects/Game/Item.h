#pragma once

class Item : public BoxCollider{

public:
	Item();
	~Item();
	void AddObject(InteriorObject* obj);
//	void AddObject();


private:
	vector<InteriorObject*> objects;
	vector<ModelMesh*> meshes;

protected:
	string id;
	string name;
	bool stackable = true;

	virtual void Use() {}
	virtual void RenderIcon() {}
	virtual ~Item() = default;
};
