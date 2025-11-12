#include "Framework.h"

Room::Room()
{

	floor = InteriorManager::Get()->Add("SM_Floor", {0,0,0});
	floor->SetParent(this);
	walls.reserve(4);
	float half = size / 2.0f;

	// 방향별 위치와 회전 정의 (북, 동, 남, 서)
	std::vector<std::pair<Vector3, Vector3>> wallTransforms;
	float r = PI / 180;
	wallTransforms.push_back(std::make_pair(Vector3(0, half, half), Vector3(0, 0, 0)));   // 북쪽 (+Z)
	wallTransforms.push_back(std::make_pair(Vector3(half, half, 0), Vector3(0,r*90, 0)));   // 동쪽 (+X)
	wallTransforms.push_back(std::make_pair(Vector3(0, half, -half), Vector3(0, r*180, 0)));   // 남쪽 (-Z)
	wallTransforms.push_back(std::make_pair(Vector3(-half, half, 0), Vector3(0, r*270, 0)));   // 서쪽 (-X)

	for (int i=0;i < 4;i++) {
		const Vector3& pos = wallTransforms[i].first;
		const Vector3& rot = wallTransforms[i].second;		
		InteriorObject* wall = InteriorManager::Get()->Add("SM_Wall_1A", pos, rot);
		wall->SetParent(this);
		walls.push_back(wall);
	}
}

Room::~Room()
{

}

bool Room::IsRayCollision(const Ray& ray, RayHit* hitInfo)
{
	
	if (floor->IsRayCollision(ray, hitInfo)) return true;

	for (int i = 0;i < 4;i++) {
		if (walls[i] == nullptr) continue;
		if (walls[i]->IsRayCollision(ray, hitInfo)) return true;
	}

	return false;
}
void Room::DisableWall(int direction)
{
	if (direction < 0 || direction >= (int)walls.size()) return;

	if (walls[direction])
		walls[direction]->SetActive(false); // or Destroy
}