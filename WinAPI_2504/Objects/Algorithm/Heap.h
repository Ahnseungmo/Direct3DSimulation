#pragma once

class Heap
{
public:
    Heap() = default;
    ~Heap() = default;

    void Insert(Node* node);
    void UpdateUpper(int index);

    Node* DeleteRoot();
    void UpdateLower(int index);

    void Swap(int n1, int n2);

    void Clear() { openNodes.clear(); }
    bool Empty() { return openNodes.empty(); }

private:
    vector<Node*> openNodes;
};
