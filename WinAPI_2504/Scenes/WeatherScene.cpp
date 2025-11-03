#include "Framework.h"
#include "WeatherScene.h"

WeatherScene::WeatherScene()
{
	collider = new BoxCollider(Vector3(50, 50, 1));
	//particle = new Sprite(L"Resources/Textures/FX/explosion.png", 30, 30, 5, 3);
    //particle = new Rain();
    //particle = new Snow();
    //particle = new Spark(L"Resources/Textures/FX/Snow.png");
    particle = new ParticleSystem("Resources/FX/Explosion.fx");
}

WeatherScene::~WeatherScene()
{
	delete collider;
	delete particle;
}

void WeatherScene::Update()
{
    if (Input::Get()->IsKeyDown(VK_LBUTTON))
    {
        Ray ray = CAM->ScreenPointToRay(mousePos);
        RayHit hit;

        if (collider->IsRayCollision(ray, &hit))
        {
            particle->Play(hit.position);

            //Vector3 rot;
            //rot.x = atan2(hit.normal.x, hit.normal.z);
            //particle->SetLocalRotation(rot);
        }
    }

    particle->Update();
}

void WeatherScene::PreRender()
{
}

void WeatherScene::Render()
{
	particle->Render();
    collider->Render();
}

void WeatherScene::PostRender()
{
}

void WeatherScene::GUIRender()
{
    //particle->GUIRender();
}
