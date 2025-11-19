#pragma once
class ItemPreset
{
public:
    static vector<string> GetMeshes(const string& id);
    static Vector3 GetOffset(const string& id, int index);
};