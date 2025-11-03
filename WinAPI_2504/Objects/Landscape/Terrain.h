#pragma once

class Terrain : public Plane
{
private:
	

public:
	Terrain();
	~Terrain();

	void Render();

	float GetHeight(const Vector3& pos);

private:
	Texture* alphaMap;
	Texture* secondMap;
};