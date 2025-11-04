#pragma once

class InteriorObject : public BoxCollider {

public:
	InteriorObject();
	~InteriorObject();


	void Update();
	void Render();

private:
	ModelInstancing *model;



};