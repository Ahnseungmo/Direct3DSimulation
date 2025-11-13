#include "Framework.h"

Item::Item()
{
}

Item::~Item()
{

}

void Item::AddObject(InteriorObject* obj)
{
	objects.push_back(obj);
	for (auto mesh : obj->GetModel()->GetMeshes()) {
		meshes.push_back(mesh);
	}
	
	FitSize(meshes);
}
