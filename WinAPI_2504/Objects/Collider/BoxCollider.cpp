#include "Framework.h"

BoxCollider::BoxCollider(Vector3 size) : size(size)
{
    type = Type::Box;

    MakeMesh();
    mesh->CreateMesh();
}

BoxCollider::~BoxCollider()
{
	delete mesh;
}

void BoxCollider::Edit()
{
	Transform::Edit();
	if (ImGui::DragFloat3("Size", (float*)&size, 0.1f, 0.1f, 100.0f))
	{
		UpdateMesh();
	}
}

bool BoxCollider::IsRayCollision(const Ray& ray, RayHit* hitInfo)
{
	if (!IsActive()) return false;

	UpdateWorld();

	ObbDesc box;
	GetOBB(box);

	Vector3 min = box.halfSize * -1.0f;
	Vector3 max = box.halfSize;

	Vector3 delta = box.center - ray.origin;

	Vector3 D = ray.direction.GetNormalized();

	float tMin = 0.0f;
	float tMax = FLT_MAX;
	int axisIndex = 0;

	for (int i = 0; i < 3; i++)
	{
		Vector3 axis = box.axis[i];
		float e = Vector3::Dot(axis, delta);
		float f = Vector3::Dot(D, axis);

		if (GameMath::NearlyEqual(f, 0.0f))
		{
			if (min[i] > e || e > max[i])
				return false;
		}
		else
		{
			float t1 = (e + min[i]) / f;
			float t2 = (e + max[i]) / f;

			if (t1 > t2)
				swap(t1, t2);

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
			{
				tMin = t1;
				axisIndex = i;
			}

			if (tMax < tMin)
				return false;
		}
	}

	if (hitInfo)
	{
		hitInfo->isHit = true;
		hitInfo->distance = tMin;
		hitInfo->position = ray.origin + D * hitInfo->distance;
		//hitInfo->normal = (hitInfo->position - box.center).GetNormalized();
		hitInfo->normal = (Vector3::Dot(D, box.axis[axisIndex]) < 0) ?
			box.axis[axisIndex] : box.axis[axisIndex] * -1.0f;
	}

    return true;
}

bool BoxCollider::IsBoxCollision(BoxCollider* collider)
{
	ObbDesc box1, box2;
	GetOBB(box1);
	collider->GetOBB(box2);

	Vector3 D = box2.center - box1.center;

	for (UINT i = 0; i < 3; i++)
	{
		if (IsSeperateAxis(D, box1.axis[i], box1, box2)) return false;
		if (IsSeperateAxis(D, box2.axis[i], box1, box2)) return false;
	}

	for (UINT i = 0; i < 3; i++)
	{
		for (UINT j = 0; j < 3; j++)
		{
			if (box1.axis[i] == box2.axis[j]) return true;
		}
	}

	for (UINT i = 0; i < 3; i++)
	{
		for (UINT j = 0; j < 3; j++)
		{
			Vector3 cross = Vector3::Cross(box1.axis[i], box2.axis[j]);
			if (IsSeperateAxis(D, cross, box1, box2)) return false;
		}
	}

	return true;
}

bool BoxCollider::IsSphereCollision(SphereCollider* collider)
{
	ObbDesc box;
	GetOBB(box);

	Vector3 closestPointToSphere = box.center;

	for (UINT i = 0; i < 3; i++)
	{
		Vector3 direction = collider->GetGlobalPosition() - box.center;
		float length = Vector3::Dot(direction, box.axis[i]);
		float mult = (length < 0.0f) ? -1.0f : 1.0f;
		length = min(abs(length), box.halfSize[i]);
		closestPointToSphere += box.axis[i] * length * mult;
	}

	float distance = Vector3::Distance(collider->GetGlobalPosition(), closestPointToSphere);

    return distance <= collider->Radius();
}

bool BoxCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
    return false;
}

void BoxCollider::FitSize(vector<ModelMesh*> meshes)
{
	if (meshes.empty()) return;

	Vector3 minPos(FLT_MAX, FLT_MAX, FLT_MAX);
	Vector3 maxPos(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (ModelMesh* mesh : meshes)
	{
		if (!mesh) continue;

		const vector<ModelVertex>& vertices = mesh->GetData().vertices;

		for (const ModelVertex& v : vertices)
		{
			Vector3 pos = v.pos;

			minPos.x = min(minPos.x, pos.x);
			minPos.y = min(minPos.y, pos.y);
			minPos.z = min(minPos.z, pos.z);

			maxPos.x = max(maxPos.x, pos.x);
			maxPos.y = max(maxPos.y, pos.y);
			maxPos.z = max(maxPos.z, pos.z);
		}
	}

	size = maxPos - minPos;

	Vector3 center = (minPos + maxPos) * 0.5f;
	SetLocalPosition(center);

	UpdateMesh();
}

void BoxCollider::GetOBB(ObbDesc& desc)
{	
	desc.center = GetGlobalPosition();
	desc.axis[0] = GetRight();
	desc.axis[1] = GetUp();
	desc.axis[2] = GetForward();
	desc.halfSize = size * 0.5f * GetGlobalScale();
}

bool BoxCollider::IsSeperateAxis(Vector3 D, Vector3 axis, ObbDesc box1, ObbDesc box2)
{
	float distance = abs(Vector3::Dot(D, axis));

	float a = 0.0f;
	float b = 0.0f;

	for (UINT i = 0; i < 3; i++)
	{
		Vector3 temp = box1.axis[i] * box1.halfSize[i];
		a += abs(Vector3::Dot(temp, axis));
		temp = box2.axis[i] * box2.halfSize[i];
		b += abs(Vector3::Dot(temp, axis));
	}

	return distance > a + b;
}

void BoxCollider::UpdateMesh()
{
	Vector3 halfSize = size * 0.5f;
	vector<Vertex>& vertices = mesh->GetVertices();
	vertices.clear();

	vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z);
	vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z);
	vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z);
	vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z);

	vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z);
	vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z);
	vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z);
	vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z);

	mesh->UpdateVertices();
}

void BoxCollider::MakeMesh()
{
	Vector3 halfSize = size * 0.5f;
	vector<Vertex>& vertices = mesh->GetVertices();
	
	vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z);
	vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z);
	vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z);
	vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z);
    
	vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z);
	vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z);
	vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z);
	vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z);

	vector<UINT>& indices = mesh->GetIndices();
	indices = {
		0, 1, 3, 2, 4, 5, 7, 6,
		0, 3, 1, 2, 4, 7, 5, 6,
		0, 4, 1, 5, 3, 7, 2, 6
	};
}
