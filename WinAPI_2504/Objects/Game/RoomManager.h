#pragma once

class RoomManager : public Singleton<RoomManager>{
	friend class Singleton<RoomManager>;
public:
	RoomManager();
	~RoomManager();
	void Update();
	void AddRoomByNormal(Room* baseRoom, const Vector3& normal);
	bool IsRoomAtPosition(const Vector3& pos, float tolerance = 0.1f);

	void SetEditMode(bool mode) { editMode = mode; }
	bool GetEditMode() { return editMode; }
private:
	bool editMode = true;
	vector<Room*> rooms;
	float roomSpacing = 4.0f;
};