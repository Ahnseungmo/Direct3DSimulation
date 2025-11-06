#pragma once

class InteriorManager : public Singleton<InteriorManager>
{
    friend class Singleton<InteriorManager>;

private:
    struct Thumbnail
    {
        RenderTarget* renderTarget;
        DepthStencil* depthStencil;
        Camera* camera;
    };

public:
    InteriorManager();
    ~InteriorManager();

    void Update();
    void Render();
    void RenderThumbnails();

    void AddMeshType(string meshName, UINT poolSize);

    InteriorObject* Add(string meshName, Vector3 position = { 0, 0, 0 }, Vector3 rotation = { 0, 0, 0 }, Vector3 scale = { 1, 1, 1 });

    InteriorObject* GetInteriorObject(string meshName, int index) { objects.at(meshName).at(index); }

    const unordered_map<string, ModelInstancing*>& GetModelInstancing() { return instancingModels; }
    ID3D11ShaderResourceView* GetThumbnailSRV(const string& name);

    void Edit();

private:
    unordered_map<string, ModelInstancing*> instancingModels;
    unordered_map<string, vector<InteriorObject*>> objects;

    unordered_map<string, Thumbnail> thumbnails;
};