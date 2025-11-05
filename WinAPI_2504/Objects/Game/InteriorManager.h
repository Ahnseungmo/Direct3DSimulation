#pragma once

class InteriorManager : public Singleton<InteriorManager>
{
    friend class Singleton<InteriorManager>;

public:
    InteriorManager();
    ~InteriorManager();

    void Update();
    void Render();

    void AddMeshType(string meshName, UINT poolSize);

    InteriorObject* Add(string meshName, Vector3 position = { 0, 0, 0 }, Vector3 rotation = { 0, 0, 0 }, Vector3 scale = { 1, 1, 1 });

    InteriorObject* GetInteriorObject(string meshName, int index) { objects.at(meshName).at(index); }

    void Edit();

private:
    unordered_map<string, ModelInstancing*> instancingModels;
    unordered_map<string, vector<InteriorObject*>> objects;
};