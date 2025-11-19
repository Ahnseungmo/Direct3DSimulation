#include "Framework.h"

vector<string> ItemPreset::GetMeshes(const string& id)
{
    if (id == "TableSet")
        return { "Table", "ChairA", "ChairB" };

    if (id == "CupWithPlate")
        return { "Plate", "Cup" };

    return { id }; // 단일 오브젝트
}

Vector3 ItemPreset::GetOffset(const string& id, int index)
{
    if (id == "TableSet")
    {
        if (index == 0) return { 0,0,0 };       // table
        if (index == 1) return { 0.5f,0,0.5f }; // chair A
        if (index == 2) return { -0.5f,0,0.5f }; // chair B
    }

    return { 0,0,0 };
}