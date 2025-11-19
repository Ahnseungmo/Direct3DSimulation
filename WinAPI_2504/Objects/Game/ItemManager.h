#pragma once
class ItemManager : public Singleton<ItemManager>
{
    friend class Singleton<ItemManager>;

private:
    vector<Item*> items;

public:
    ItemManager() {}
    ~ItemManager();

    Item* CreateItem(string itemId);
    void Update();
    void Render();
    void Edit();

    const vector<Item*>& GetItems() const { return items; }
};
