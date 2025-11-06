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

    for (auto& pair : thumbnails)
    {
        delete pair.second.renderTarget;
        delete pair.second.depthStencil;
        delete pair.second.camera;
    }
}

void InteriorManager::Update()
{
    for (auto const& pair : objects)
    {
        bool first = true;
        for (InteriorObject* obj : pair.second)
        {
            /*
            if (first) { first = false; continue; }
            if (obj->IsActive())
            {
                obj->UpdateWorld();
            }
            */

            obj->UpdateWorld();

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
            /*
            if (first) { first = false; continue; }
            if (obj->IsActive())
            {
                obj->Render();
            }
            */

            obj->Render();

        }
    }
}

void InteriorManager::RenderThumbnails()
{
    for (auto const& pair : instancingModels)
    {
        if (thumbnails.count(pair.first) == 0) continue;

        Thumbnail& thumbnail = thumbnails.at(pair.first);
        thumbnail.renderTarget->Set(thumbnail.depthStencil);

        // 0번 샘플 오브젝트를 렌더링
        InteriorObject* sampleObject = objects.at(pair.first)[0];
        if (sampleObject)
        {
            // 썸네일 카메라로 뷰/프로젝션 설정
            thumbnail.camera->SetView();
            sampleObject->UpdateWorld();
            sampleObject->Render();
        }
    }
}

ID3D11ShaderResourceView* InteriorManager::GetThumbnailSRV(const string& name)
{
    if (thumbnails.count(name) > 0)
        return thumbnails.at(name).renderTarget->GetSRV();

    return nullptr;
}

void InteriorManager::AddMeshType(string meshName, UINT poolSize)
{
    if (instancingModels.count(meshName) > 0)
        return;

    instancingModels[meshName] = new ModelInstancing(meshName, poolSize);
    objects[meshName] = {};

    // 0번에 썸네일 및 샘플용 데이터 등록
    instancingModels[meshName]->Add();
    InteriorObject* sampleObject = new InteriorObject(meshName);
    sampleObject->SetTag(meshName + "_0");
    objects[meshName].push_back(sampleObject);

    // 썸네일 렌더링용 리소스 생성
    Thumbnail& thumbnail = thumbnails[meshName];
    thumbnail.renderTarget = new RenderTarget(100, 100);
    thumbnail.depthStencil = new DepthStencil(100, 100);
    thumbnail.camera = new Camera();
    thumbnail.camera->SetLocalPosition(0, 0, -1.0f);


    BoxCollider* collider = sampleObject; 
    Vector3 size = collider->GetSize();
    float maxSize = max(size.x, max(size.y, size.z));
    

    Vector3 center = collider->GetGlobalPosition();
    thumbnail.camera->SetLocalPosition(center.x, center.y + size.y * 0.5f, center.z - maxSize * 2.0f);
    thumbnail.camera->UpdateWorld();

    sampleObject->SetInstancing(false);

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
    for (auto const& pair : instancingModels)
    {
        pair.second->Edit();


    }

    for (auto pair : objects)
    {
 //       /*
        for (InteriorObject* obj : pair.second) // 0번 샘플 오브젝트도 Edit 창에 표시됨
            if(obj != nullptr)
                obj->Edit();
   //     */
    }

}