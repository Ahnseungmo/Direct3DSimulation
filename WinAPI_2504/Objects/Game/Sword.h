#pragma once

class Sword : public BoxCollider
{
public:
	Sword();
	~Sword();

	void Update();
	void Render();
	void Edit();

private:
	Model* model;
};