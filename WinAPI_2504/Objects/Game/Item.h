#pragma once
class Item
{
public:
    Item(string id);
    ~Item();

    void AddPart(string meshName, Vector3 pos, Vector3 rot, Vector3 scale);

    const vector<InteriorObject*>& GetObjects() const { return parts; }

    void Update();
    void Render();
    void Edit();

private:
    string id;
    vector<InteriorObject*> parts;
};
