#pragma once

class InteriorObject : public BoxCollider {
public:
	InteriorObject();
	InteriorObject(string meshName);

	~InteriorObject();

	void Update();
	void Render();

	void SetInstancing(bool isInstanced) { this->isInstanced = isInstanced; }

	Model* GetModel() { return model; }

private:
	Model* model;

	bool isInstanced = false;

};