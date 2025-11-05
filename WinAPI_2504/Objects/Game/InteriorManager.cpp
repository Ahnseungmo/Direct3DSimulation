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
        bool first = true;
        for (InteriorObject* obj : pair.second)
        {
            if (first) { first = false; continue; }
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
    bool first = true;
    for (auto const& pair : instancingModels)
    {
        if (first) { first = false; continue; }
        pair.second->Render();
    }

    for (auto const& pair : objects)
    {
        bool first = true;
        for (InteriorObject* obj : pair.second)
        {
            if (first) { first = false; continue; }
            if (obj->IsActive())
            {
                obj->Render();
            }
        }
    }
}


void InteriorManager::AddMeshType(string meshName, UINT poolSize)
{
    if (instancingModels.count(meshName) > 0)
        return;

    instancingModels[meshName] = new ModelInstancing(meshName, poolSize);
    objects[meshName] = {};

    /////////0번에 샘플 데이터 등록
    instancingModels[meshName]->Add();
    objects[meshName].push_back(new InteriorObject(meshName));
    objects[meshName].back()->SetTag(meshName + "_0");

}

InteriorObject* InteriorManager::Add(string meshName, Vector3 position, Vector3 rotation, Vector3 scale)
{
    if (instancingModels.count(meshName) == 0)
        return nullptr;

    Transform* instanceTransform = instancingModels[meshName]->Add();
    if (!instanceTransform) return nullptr; 
    InteriorObject* obj = new InteriorObject(meshName);
    instanceTransform->SetParent(obj);
    obj->SetInstancing(true);

    obj->SetLocalPosition(position);
    obj->SetLocalRotation(rotation);
    obj->SetLocalScale(scale);
    obj->SetTag(meshName + "q" + to_string(objects[meshName].size()));
    objects[meshName].push_back(obj);
    return obj;
}


void InteriorManager::Edit()
{
//    iterator it;
    for (auto const& pair : instancingModels)
    {
        pair.second->Edit();


    }

    for (auto pair : objects)
    {
 //       /*
        for (InteriorObject* obj : pair.second)
            if(obj != nullptr)
                obj->Edit();
   //     */
    }
}