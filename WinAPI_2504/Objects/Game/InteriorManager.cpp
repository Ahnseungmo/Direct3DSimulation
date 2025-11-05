#include "Framework.h"
#include "Objects/Game/InteriorManager.h"

InteriorManager::InteriorManager()
{
}

InteriorManager::~InteriorManager()
{
    for (auto pair : instancingModels)
        delete pair.second;

    for (auto pair : objects)
    {
        for (InteriorObject* obj : pair.second)
            delete obj;
    }
}

void InteriorManager::Update()
{
    for (auto const& pair : objects)
    {
        for (InteriorObject* obj : pair.second)
        {
            if (obj->IsActive())
            {
                obj->UpdateWorld();
            }
        }
    }

    for (auto const& pair : instancingModels)
    {
        pair.second->Update();
    }
}

void InteriorManager::Render()
{
    for (auto const& pair : instancingModels)
    {
        pair.second->Render();
    }
}

void InteriorManager::AddMeshType(string meshName, UINT poolSize)
{
    if (instancingModels.count(meshName) > 0)
        return;

    instancingModels[meshName] = new ModelInstancing(meshName, poolSize);
    objects[meshName] = {};
}

InteriorObject* InteriorManager::Add(string meshName, Vector3 position, Vector3 rotation, Vector3 scale)
{
    if (instancingModels.count(meshName) == 0)
        return nullptr;

    Transform* instanceTransform = instancingModels[meshName]->Add();
    if (!instanceTransform) return nullptr; 
    InteriorObject* obj = new InteriorObject(meshName);
    obj->SetParent(instanceTransform);
    obj->SetInstancing(true);

    objects[meshName].push_back(obj);
    return obj;
}