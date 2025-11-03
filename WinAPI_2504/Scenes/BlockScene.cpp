#include "Framework.h"
#include "BlockScene.h"

BlockScene::BlockScene()
{
	LoadTextures();
	CreateBlocks();	

	steve = new Steve();

	light = Environment::Get()->GetLight(0);
	light->type = 2;

	LightBuffer::Light* pointLight = Environment::Get()->AddLight();
	pointLight->type = 1;
	pointLight->position = Vector3(0, 5, 0);
}

BlockScene::~BlockScene()
{
	for (Block* block : blocks)
		delete block;

	delete steve;
}

void BlockScene::Update()
{
	if (Input::Get()->IsKeyDown(VK_LBUTTON))
		Mine();

	if (Input::Get()->IsKeyDown(VK_RBUTTON))
		Build();

	light->position = CAM->GetGlobalPosition();
	light->direction = CAM->GetForward();

	steve->Update();
}

void BlockScene::PreRender()
{
}

void BlockScene::Render()
{
	for (Block* block : blocks)
		block->Render();

	steve->Render();
}

void BlockScene::PostRender()
{
}

void BlockScene::GUIRender()
{
}

void BlockScene::LoadTextures()
{
	textures.push_back(Texture::Add(L"Resources/Textures/Landscape/Box.png"));
	textures.push_back(Texture::Add(L"Resources/Textures/Landscape/Bricks.png"));
	textures.push_back(Texture::Add(L"Resources/Textures/Landscape/Dirt.png"));
	textures.push_back(Texture::Add(L"Resources/Textures/Landscape/Dirt2.png"));
	textures.push_back(Texture::Add(L"Resources/Textures/Landscape/Dirt3.png"));
}

void BlockScene::CreateBlocks()
{
	blocks.reserve(11 * 11 * 3);

	for (float y = 0.0f; y < 3.0f; y += 1.0f)
	{
		for (float z = -5.0f; z <= 5.0f; z += 1.0f)
		{
			for (float x = -5.0f; x <= 5.0f; x += 1.0f)
			{
				Block* block = new Block();
				block->SetLocalPosition(x, y, z);

				int index = GameMath::Random(0, textures.size());
				block->GetCube()->GetMaterial()->SetDiffuseMap(textures[index]);

				block->Update();

				blocks.push_back(block);				
			}
		}
	}
}

void BlockScene::Mine()
{
	Block* block = GetClosestBlock();

	if (block != nullptr)
	{
		block->SetActive(false);		
		mineBlocks.push(block);
	}
}

void BlockScene::Build()
{
	if (mineBlocks.empty()) return;

	Vector3 normal;

	Block* closestBlock = GetClosestBlock(&normal);
	if (closestBlock == nullptr) return;

	Block* block = mineBlocks.front();
	mineBlocks.pop();	

	Vector3 pos = closestBlock->GetGlobalPosition() + normal;
	block->SetLocalPosition(pos);
	block->Update();
	block->SetActive(true);
}

Block* BlockScene::GetClosestBlock(Vector3* normal)
{
	Ray ray = CAM->ScreenPointToRay(mousePos);
	RayHit hitInfo;

	float minDistance = FLT_MAX;
	Block* closestBlock = nullptr;

	for (Block* block : blocks)
	{
		if (block->IsRayCollision(ray, &hitInfo))
		{
			if (hitInfo.distance < minDistance)
			{
				minDistance = hitInfo.distance;
				closestBlock = block;

				if (normal != nullptr)
					*normal = hitInfo.normal;
			}
		}
	}

	return closestBlock;
}
