#include "Framework.h"

Camera::Camera()
{
	tag = "Camera";

	viewBuffer = new ViewBuffer();
}

Camera::~Camera()
{
	delete viewBuffer;
}

void Camera::Update()
{
	projection = Environment::Get()->GetPerspective();

	FrusutmUpdate();

    if (target)
        FollowMode();
    else
	    FreeMode();

	UpdateWorld();		
}

void Camera::Edit()
{
	ImGui::DragFloat("MoveSpeed", &moveSpeed, 0.1f, 0.1f, 100.0f);
	ImGui::DragFloat("RotSpeed", &rotSpeed, 0.1f, 0.1f, 100.0f);

    if (target)
    {
        ImGui::DragFloat("Distance", &distance);
        ImGui::DragFloat("Height", &height);

        ImGui::DragFloat("MoveDamping", &moveDamping, 0.1f);
        ImGui::DragFloat("RotDamping", &rotDamping, 0.1f);

        ImGui::DragFloat3("FocusOffset", (float*)&focusOffset, 0.1f);

        ImGui::Checkbox("LookAtTargetX", &isLookAtTargetX);
        ImGui::Checkbox("LookAtTargetY", &isLookAtTargetY);

        //ImGui::InputText("File", file, 128);
        //
        //if (ImGui::Button("Save"))
        //    TargetOptionSave(file);
        //
        //ImGui::SameLine();
        //
        //if (ImGui::Button("Load"))
        //    TargetOptionLoad(file);
    }

    Transform::Edit();
}

void Camera::SetView(UINT slot)
{
    view = XMMatrixInverse(nullptr, world);
    viewBuffer->Set(view, world);
    viewBuffer->SetVS(slot);
}

Ray Camera::ScreenPointToRay(Vector3 screenPos)
{
    Vector3 screenSize(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f);    

    Vector2 point;
	point.x = (screenPos.x / screenSize.x) * 2.0f - 1.0f;
	point.y = (screenPos.y / screenSize.y) * 2.0f - 1.0f;
	//-> 0~screen -> -1~1

    Float4x4 temp;
    XMStoreFloat4x4(&temp, projection);

	screenPos.x = point.x / temp._11;
	screenPos.y = point.y / temp._22;
    screenPos.z = 1.0f;

    screenPos = XMVector3TransformNormal(screenPos, world);

    Ray ray;
	ray.origin = localPosition;
	ray.direction = screenPos.GetNormalized();

    return ray;
}

Vector3 Camera::WorldToScreenPoint(Vector3 worldPos)
{
    Vector3 screenPos;

    screenPos = worldPos * view;
	screenPos = screenPos * projection;

	screenPos = (screenPos + Vector3(1.0f, 1.0f, 1.0f)) * 0.5f;//-1~1 -> 0~1

	screenPos.x *= SCREEN_WIDTH;
	screenPos.y *= SCREEN_HEIGHT;

    return screenPos;
}

bool Camera::ContainPoint(Vector3 point)
{
    FOR(6)
    {
        Vector3 dot = XMPlaneDotCoord(planes[i], point);

        if (dot.x < 0.0f)
            return false;
    }

    return true;
}

bool Camera::ContainSphere(Vector3 center, float radius)
{
    return false;
}

void Camera::FreeMode()
{
    Vector3 delta = mousePos - prevMousePos;
    prevMousePos = mousePos;

    if (Input::Get()->IsKeyPress(VK_RBUTTON))
    {
        if (Input::Get()->IsKeyPress('W'))
            Translate(forward * moveSpeed * DELTA);
        if (Input::Get()->IsKeyPress('S'))
            Translate(-forward * moveSpeed * DELTA);
        if (Input::Get()->IsKeyPress('E'))
            Translate(up * moveSpeed * DELTA);
        if (Input::Get()->IsKeyPress('Q'))
            Translate(-up * moveSpeed * DELTA);
        if (Input::Get()->IsKeyPress('A'))
            Translate(-right * moveSpeed * DELTA);
        if (Input::Get()->IsKeyPress('D'))
            Translate(right * moveSpeed * DELTA);

        Rotate(Vector3::Right(), -delta.y * rotSpeed * DELTA);
        Rotate(Vector3::Up(), delta.x * rotSpeed * DELTA);
    }
}

void Camera::FollowMode()
{
    destRot = GameMath::Lerp<float>(destRot, target->GetLocalRotation().y, rotDamping * DELTA);
    rotMatrix = XMMatrixRotationY(destRot);

    Vector3 foraward = Vector3::Forward() * rotMatrix;

	destPos = target->GetGlobalPosition() - foraward * distance;
	destPos.y += height;

	localPosition = GameMath::Lerp<Vector3>(localPosition, destPos, moveDamping * DELTA);

    Vector3 offset = focusOffset * rotMatrix;
	Vector3 targetPos = target->GetGlobalPosition() + offset;

    Vector3 dir = targetPos - localPosition;
    dir.Normalize();
    Vector3 dirXZ = Vector3(dir.x, 0.0f, dir.z);
    dirXZ.Normalize();

	float dot = Vector3::Dot(dir, dirXZ);

    if(isLookAtTargetX)
		localRotation.x = acos(dot);
    if(isLookAtTargetY)
        localRotation.y = atan2(foraward.x, foraward.z);

    //float destRotY = atan2(foraward.x, foraward.z);
	//localRotation.y = GameMath::Lerp<float>(localRotation.y, destRotY, rotDamping * DELTA);
	
}

void Camera::FrusutmUpdate()
{
    Float4x4 VP;
    XMStoreFloat4x4(&VP, view * projection);

    //Left
    a = VP._14 + VP._11;
    b = VP._24 + VP._21;
    c = VP._34 + VP._31;
    d = VP._44 + VP._41;
    planes[0] = XMVectorSet(a, b, c, d);

    //Right
    a = VP._14 - VP._11;
    b = VP._24 - VP._21;
    c = VP._34 - VP._31;
    d = VP._44 - VP._41;
    planes[1] = XMVectorSet(a, b, c, d);

    //Bottom
    a = VP._14 + VP._12;
    b = VP._24 + VP._22;
    c = VP._34 + VP._32;
    d = VP._44 + VP._42;
    planes[2] = XMVectorSet(a, b, c, d);

    //Top
    a = VP._14 - VP._12;
    b = VP._24 - VP._22;
    c = VP._34 - VP._32;
    d = VP._44 - VP._42;
    planes[3] = XMVectorSet(a, b, c, d);

    //Near
    a = VP._14 + VP._13;
    b = VP._24 + VP._23;
    c = VP._34 + VP._33;
    d = VP._44 + VP._43;
    planes[4] = XMVectorSet(a, b, c, d);

    //Far
    a = VP._14 - VP._13;
    b = VP._24 - VP._23;
    c = VP._34 - VP._33;
    d = VP._44 - VP._43;
    planes[5] = XMVectorSet(a, b, c, d);

    FOR(6)
        planes[i] = XMPlaneNormalize(planes[i]);
}
