#include "Framework.h"
#include "SphereScene.h"

SphereScene::SphereScene()
{
	LoadTextures();

	crossHair = new Quad(L"Resources/Textures/UI/CrossHair.png");
	crossHair->SetLocalPosition(CENTER_X, CENTER_Y, 0.0f);
	crossHair->UpdateWorld();

	balls.reserve(BALL_COUNT);
	for (UINT i = 0; i < BALL_COUNT; i++)
	{
		balls.push_back(new GameBall());
		balls.back()->SetActive(false);
	}
}

SphereScene::~SphereScene()
{
	delete crossHair;	

	for (GameBall* ball : balls)
		delete ball;
}

void SphereScene::Update()
{
	Spawn();

	CheckCollision();
}

void SphereScene::PreRender()
{
}

void SphereScene::Render()
{
	for (GameBall* ball : balls)
		ball->Render();
}

void SphereScene::PostRender()
{
	crossHair->Render();
}

void SphereScene::GUIRender()
{
}

void SphereScene::Spawn()
{
	spawnTimer += DELTA;

	if (spawnTimer >= SPAWN_TIME)
	{
		spawnTimer -= SPAWN_TIME;

		for (GameBall* ball : balls)
		{
			if (!ball->IsActive())
			{
				ball->SetActive(true);

				float x = GameMath::Random(-MAX_RANGE, +MAX_RANGE);
				float z = GameMath::Random(-MAX_RANGE, +MAX_RANGE);

				ball->SetLocalPosition(x, 0.0f, z);
				int index = GameMath::Random(0, ballTextures.size());
				ball->GetSphere()->GetMaterial()->SetDiffuseMap(ballTextures[index]);

				float scale = GameMath::Random(0.5f, 2.0f);
				ball->SetLocalScale(scale, scale, scale);				

				ball->Update();
				return;
			}
		}
	}
}

void SphereScene::LoadTextures()
{
	ballTextures.push_back(Texture::Add(L"Resources/Textures/Landscape/Box.png"));
	ballTextures.push_back(Texture::Add(L"Resources/Textures/Landscape/Bricks.png"));
	ballTextures.push_back(Texture::Add(L"Resources/Textures/Landscape/Dirt.png"));
	ballTextures.push_back(Texture::Add(L"Resources/Textures/Landscape/Dirt2.png"));
	ballTextures.push_back(Texture::Add(L"Resources/Textures/Landscape/Dirt3.png"));
}

void SphereScene::CheckCollision()
{
	Ray ray = CAM->ScreenPointToRay(Vector3(CENTER_X, CENTER_Y, 0.0f));
	
	bool hitSomething = false;

	for (GameBall* ball : balls)
	{
		if (ball->IsActive() && ball->IsRayCollision(ray, &hit))
		{
			crossHair->GetMaterial()->SetColor(1, 0, 0, 1);
			hitSomething = true;
			break;
		}
	}

	if (hitSomething == false)
		crossHair->GetMaterial()->SetColor(1, 1, 1, 1);
}
