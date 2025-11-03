#include "Framework.h"
#include "CharacterScene.h"

CharacterScene::CharacterScene()
{
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();

	koreanFont = io.Fonts->AddFontFromFileTTF(
		"Resources/Fonts/BMJUA.ttf", 50.0f, nullptr, io.Fonts->GetGlyphRangesKorean()
	);

	naruto = new Naruto();
}

CharacterScene::~CharacterScene()
{
	delete naruto;
}

void CharacterScene::Update()
{
	naruto->Update();
}

void CharacterScene::PreRender()
{
}

void CharacterScene::Render()
{
	naruto->Render();
}

void CharacterScene::PostRender()
{
}

void CharacterScene::GUIRender()
{
	naruto->Edit();

	ImDrawList* draw_list = ImGui::GetForegroundDrawList();	

	Vector3 offset = Vector3(0.0f, 3.0f, 0.0f);
	Vector3 screenPos = CAM->WorldToScreenPoint(naruto->GetGlobalPosition() + offset);
	screenPos.y = SCREEN_HEIGHT - screenPos.y;	

	draw_list->AddText(koreanFont, 50, ImVec2(screenPos.x - 50, screenPos.y),
		IM_COL32(255, 255, 0, 255), u8"³ª·çÅä");
}
