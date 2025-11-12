#pragma once

class Room : public Transform
{
public:
	enum WallDirection
	{
		NORTH = 0,  // +Z
		EAST = 1,  // +X
		SOUTH = 2,  // -Z
		WEST = 3   // -X
	};


	Room();
	~Room();
	bool IsRayCollision(const Ray& ray, RayHit* hitInfo);
	void DisableWall(int direction);
private:
	float size = 4.0f;
	vector<InteriorObject*> walls;
	InteriorObject* floor;
};