#include "Framework.h"
#include "ItemManager.h"
#include "InteriorManager.h"

ItemManager::~ItemManager()
{
    for (Item* item : items)
        delete item;
}

Item* ItemManager::CreateItem(string itemId)
{
    Item* item = nullptr;

    if (itemId == "TableSet")
        item = new TableSet();
    else
        item = new Item(itemId);

    items.push_back(item);

    // Item 내부의 파츠들을 InteriorManager로 넘김
    for (InteriorObject* obj : item->GetObjects())
    {
        InteriorManager::Get()->Add(
            obj->GetModel()->GetName(),
            obj->GetLocalPosition(),
            obj->GetLocalRotation(),
            obj->GetLocalScale()
        );
    }

    return item;
}


void ItemManager::Update()
{
    for (Item* item : items)
        item->Update();
}

void ItemManager::Render()
{
    for (Item* item : items)
        item->Render(); // 필요 없다면 비워도 됨
}

void ItemManager::Edit()
{
    for (Item* item : items)
        item->Edit();
}
