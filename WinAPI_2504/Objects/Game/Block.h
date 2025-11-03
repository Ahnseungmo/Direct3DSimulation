#pragma once

class Block : public BoxCollider
{
public:
	Block();
	~Block();
	
	void Update();
	void Render();

	Cube* GetCube() { return cube; }

private:
	Cube* cube;
};