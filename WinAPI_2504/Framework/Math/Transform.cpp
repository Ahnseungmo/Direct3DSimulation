#include "Framework.h"

Transform::Transform()
{
    world = XMMatrixIdentity();
}

Transform::~Transform()
{
}

void Transform::UpdateWorld()
{
	world = XMMatrixTransformation(pivot,
		XMQuaternionIdentity(), localScale, pivot,
		XMQuaternionRotationRollPitchYawFromVector(localRotation),
		localPosition);

    if (parent)
        world *= parent->world;

    XMStoreFloat4x4(&matWorld, world);

    right = { matWorld._11, matWorld._12, matWorld._13 };
    up = { matWorld._21, matWorld._22, matWorld._23 };
	forward = { matWorld._31, matWorld._32, matWorld._33 };

    XMVECTOR outS, outR, outT;
    XMMatrixDecompose(globalScale.GetValue(), &outR, globalPosition.GetValue(), world);
}

void Transform::Edit()
{
	if (ImGui::TreeNode((tag + "_Transform").c_str()))
	{
		ImGui::DragFloat3("Pos", (float*)&localPosition, 1.0f);

		Float3 rot;
		rot.x = XMConvertToDegrees(localRotation.x);
		rot.y = XMConvertToDegrees(localRotation.y);
		rot.z = XMConvertToDegrees(localRotation.z);

		ImGui::DragFloat3("Rot", (float*)&rot, 0.1f, -180.0f, 180.0f);

		localRotation.x = XMConvertToRadians(rot.x);
		localRotation.y = XMConvertToRadians(rot.y);
		localRotation.z = XMConvertToRadians(rot.z);

		ImGui::DragFloat3("Scale", (float*)&localScale, 0.1f);

		if (ImGui::Button("Save"))
			Save();

		ImGui::SameLine();

		if (ImGui::Button("Load"))
			Load();

		ImGui::TreePop();
	}
}

void Transform::Save()
{
	BinaryWriter* writer = new BinaryWriter("Resources/TextData/Transforms/" + tag + ".srt");

	writer->Float(localPosition.x);
	writer->Float(localPosition.y);
	writer->Float(localPosition.z);

	writer->Float(localRotation.x);
	writer->Float(localRotation.y);
	writer->Float(localRotation.z);

	writer->Float(localScale.x);
	writer->Float(localScale.y);
	writer->Float(localScale.z);

	delete writer;
}

void Transform::Load()
{
	BinaryReader* reader = new BinaryReader("Resources/TextData/Transforms/" + tag + ".srt");

	if (reader->IsFailed())
		return;

	localPosition.x = reader->Float();
	localPosition.y = reader->Float();
	localPosition.z = reader->Float();

	localRotation.x = reader->Float();
	localRotation.y = reader->Float();
	localRotation.z = reader->Float();

	localScale.x = reader->Float();
	localScale.y = reader->Float();
	localScale.z = reader->Float();

	delete reader;
}

bool Transform::IsActive()
{
	if (parent == nullptr)
		return isActive;

	if (isActive == false)
		return false;

	return parent->IsActive();
}
