#include "Framework.h"
#include "RoomManager.h"

RoomManager::RoomManager()
{
    Room* firstRoom = new Room();
    rooms.push_back(firstRoom);
}

RoomManager::~RoomManager()
{
    for (Room* room : rooms)
        delete room;
}

void RoomManager::Update()
{
    if (editMode && Input::Get()->IsKeyDown(VK_LBUTTON))
    {
        Ray ray = CAM->ScreenPointToRay(mousePos);

        Room* hitRoom = nullptr;
        Vector3 hitNormal;
        RayHit closestHit;
        closestHit.distance = FLT_MAX; // 가장 가까운 방을 찾기 위해 초기화

        // 모든 방 검사
        for (Room* room : rooms)
        {
            RayHit hitInfo;
            if (room->IsRayCollision(ray, &hitInfo))
            {
                if (hitInfo.distance < closestHit.distance)
                {
                    closestHit = hitInfo;
                    hitRoom = room;
                    hitNormal = hitInfo.normal;
                }
            }
        }

        // 클릭된 방이 있다면
        if (hitRoom)
            AddRoomByNormal(hitRoom, hitNormal);
    }
}

// 이미 같은 위치에 Room이 있는지 확인하는 함수
bool RoomManager::IsRoomAtPosition(const Vector3& pos, float tolerance)
{
    for (Room* room : rooms)
    {
        Vector3 roomPos = room->GetGlobalPosition();
        if (Vector3::Distance(roomPos, pos) < tolerance)
            return true;
    }
    return false;
}
void RoomManager::AddRoomByNormal(Room* baseRoom, const Vector3& normal)
{
    if (!baseRoom) return;

    Vector3 basePos = baseRoom->GetGlobalPosition();
    Vector3 newPos = basePos;

    int baseWallDir = -1;
    int newWallDir = -1;

    // 클릭한 벽 기준으로 새 방 위치 설정
    if (fabs(normal.z - 1.0f) < 0.1f) {       // 북쪽 벽 클릭
        newPos.z += roomSpacing;              // 북쪽으로 새 방 생성
        baseWallDir = Room::NORTH;
        newWallDir = Room::SOUTH;
    }
    else if (fabs(normal.z + 1.0f) < 0.1f) {  // 남쪽 벽 클릭
        newPos.z -= roomSpacing;              // 남쪽으로 새 방 생성
        baseWallDir = Room::SOUTH;
        newWallDir = Room::NORTH;
    }
    else if (fabs(normal.x - 1.0f) < 0.1f) {  // 동쪽 벽 클릭
        newPos.x += roomSpacing;              // 동쪽으로 새 방 생성
        baseWallDir = Room::EAST;
        newWallDir = Room::WEST;
    }
    else if (fabs(normal.x + 1.0f) < 0.1f) {  // 서쪽 벽 클릭
        newPos.x -= roomSpacing;              // 서쪽으로 새 방 생성
        baseWallDir = Room::WEST;
        newWallDir = Room::EAST;
    }
    else
    {
        // 클릭한 면이 유효하지 않으면 생성 안함
        return;
    }

    // 이미 그 자리에 방이 있으면 생성하지 않음
    if (IsRoomAtPosition(newPos, 0.1f))
        return;

    // 새 Room 생성
    Room* newRoom = new Room();
    newRoom->SetLocalPosition(newPos);
    newRoom->UpdateWorld();
    rooms.push_back(newRoom);

    // 서로 맞닿은 벽 비활성화
    baseRoom->DisableWall(baseWallDir);
    newRoom->DisableWall(newWallDir);
}
