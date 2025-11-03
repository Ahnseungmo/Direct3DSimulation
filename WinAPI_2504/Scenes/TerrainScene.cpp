#include "Framework.h"
#include "TerrainScene.h"

TerrainScene::TerrainScene()
{
	skybox = new Skybox(L"Resources/Textures/Landscape/Snow_ENV.dds");
	terrain = new Terrain();

    /*trees.reserve(SIZE);
    FOR(SIZE)
    {
        Float2 size;
        size.x = GameMath::Random(3.0f, 10.0f);
        size.y = GameMath::Random(3.0f, 10.0f);

        Quad* tree = new Quad(size);
		tree->GetMaterial()->SetShader(L"Basic/Texture.hlsl");
        tree->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Landscape/Tree.png");
        Vector3 pos;
        pos.x = GameMath::Random(0.0f, terrain->GetSize().x);
        pos.z = GameMath::Random(0.0f, terrain->GetSize().y);
        pos.y = terrain->GetHeight(pos) + size.y * 0.5f;
        tree->SetLocalPosition(pos);

        trees.push_back(tree);
    }*/

    material = new Material(L"Geometry/Billboard.hlsl");
    material->SetDiffuseMap(L"Resources/Textures/Landscape/Tree.png");

    geometryShader = Shader::AddGS(L"Geometry/Billboard.hlsl");

    vertices.resize(SIZE);

    FOR(SIZE)
    {
        vertices[i].u = GameMath::Random(3.0f, 10.0f);
        vertices[i].v = GameMath::Random(3.0f, 10.0f);

        vertices[i].x = GameMath::Random(0.0f, terrain->GetSize().x);
        vertices[i].z = GameMath::Random(0.0f, terrain->GetSize().y);
		Vector3 pos = { vertices[i].x, 0.0f, vertices[i].z };
        vertices[i].y = terrain->GetHeight(pos) + vertices[i].v * 0.5f;
    }

    vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexUV), SIZE);
}

TerrainScene::~TerrainScene()
{
	delete skybox;
	delete terrain;

    //for (Quad* tree : trees)
    //    delete tree;

    delete material;
    delete vertexBuffer;
}

void TerrainScene::Update()
{
    //for (Quad* tree : trees)
    //{
    //    //tree->SetLocalRotation(CAM->GetLocalRotation());
    //    tree->SetLocalRotation({ 0, CAM->GetLocalRotation().y, 0 });
    //    //Vector3 dir = tree->GetLocalPosition() - CAM->GetLocalPosition();
    //    //tree->SetLocalRotation({ 0, atan2(dir.x, dir.z), 0 });
    //
    //    tree->UpdateWorld();
    //}
}

void TerrainScene::PreRender()
{
}

void TerrainScene::Render()
{
	//skybox->Render();
	terrain->Render();	

	Environment::Get()->SetAlphaBlend(true);
    //Environment::Get()->SetAlphaToCoverage();
    Environment::Get()->SetDepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);

   //for (Quad* tree : trees)
   //   tree->Render();

    vertexBuffer->Set(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
    material->Set();
    geometryShader->Set();

    DC->Draw(SIZE, 0);

   Environment::Get()->SetAlphaBlend(false);

   DC->GSSetShader(nullptr, nullptr, 0);
}

void TerrainScene::PostRender()
{
}

void TerrainScene::GUIRender()
{
}
