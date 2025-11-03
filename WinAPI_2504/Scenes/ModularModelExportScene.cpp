#include "Framework.h"
#include "ModularModelExportScene.h"


ModularModelExportScene::ModularModelExportScene()
{
}

ModularModelExportScene::~ModularModelExportScene()
{
	delete exporter;
}

void ModularModelExportScene::Update()
{
}

void ModularModelExportScene::PreRender()
{
}

void ModularModelExportScene::Render()
{
}

void ModularModelExportScene::PostRender()
{
}

void ModularModelExportScene::GUIRender()
{
	ImGui::Begin("Modular Model Exporter");

	if (ImGui::Button("Open FBX File"))
	{
		DIALOG->OpenDialog("ChooseFBXForModular", "Choose a FBX File", ".*[Ff][Bb][Xx]");
	}


	if (DIALOG->Display("ChooseFBXForModular"))
	{
		if (DIALOG->IsOk())
		{
			string fbxPath = DIALOG->GetFilePathName();
			string name = Utility::GetFileNameWithoutExtension(fbxPath);

			delete exporter; // 이전 익스포터 정리
			exporter = new ModularModelExporter(name, fbxPath);

			// 메쉬 목록 체크 상태 초기화
			const aiScene* scene = exporter->GetScene();
			if (scene)
			{
				meshCheckStates.assign(scene->mNumMeshes, false);
			}
		}
		DIALOG->Close();
	}

	if (exporter == nullptr)
	{
		ImGui::End();
		return;
	}

	ImGui::Separator();
	ImGui::Text("FBX Path: %s", exporter->GetFile().c_str());
	ImGui::Text("Model Name: %s", exporter->GetName().c_str());
	ImGui::Separator();

	// 3. 재질 및 애니메이션 익스포트 버튼
	if (ImGui::Button("Export Material"))
	{
		exporter->ExportMaterial();
	}
	ImGui::SameLine();
	if (ImGui::Button("Export Animation Clip"))
	{
		string clipName = Utility::GetFileNameWithoutExtension(exporter->GetFile());
		exporter->ExportClip(clipName);
	}

	ImGui::Separator();

	// 4. 모듈러 파츠 익스포트 UI
	ImGui::Text("Modular Part Export");
	static char partName[128] = "PartName";
	ImGui::InputText("Part Name", partName, sizeof(partName));

	if (ImGui::Button("Export Selected Meshes as Part"))
	{
		vector<UINT> selectedIndices;
		for (UINT i = 0; i < meshCheckStates.size(); ++i)
		{
			if (meshCheckStates[i])
			{
				selectedIndices.push_back(i);
			}
		}

		if (!selectedIndices.empty() && strlen(partName) > 0 && strcmp(partName, "PartName") != 0)
		{
			exporter->ExportMeshPart(partName, selectedIndices);
		}
	}

	// 5. FBX에 포함된 메쉬 목록 표시
	ImGui::Separator();
	ImGui::Text("Mesh List");
	const aiScene* scene = exporter->GetScene();
	if (scene)
	{
		for (UINT i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[i];
			bool isChecked = meshCheckStates[i];
			if (ImGui::Checkbox(mesh->mName.C_Str(), &isChecked))
			{
				meshCheckStates[i] = isChecked;
			}
		}
	}

	ImGui::End();
}