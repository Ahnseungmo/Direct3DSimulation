#include "Framework.h"

void Heap::Insert(Node* node)
{
    openNodes.push_back(node);
    UpdateUpper(openNodes.size() - 1);
}

void Heap::UpdateUpper(int index)
{
    int curIndex = index;
    int parent = (curIndex - 1) / 2;

    while (curIndex != 0)
    {
        if (openNodes[parent]->GetCost() <= openNodes[curIndex]->GetCost())
            return;

        Swap(curIndex, parent);
        curIndex = parent;
        parent = (curIndex - 1) / 2;
    }
}

Node* Heap::DeleteRoot()
{
    Node* root = openNodes.front();
    Swap(0, openNodes.size() - 1);
    openNodes.pop_back();
    UpdateLower(0);

    return root;
}

void Heap::UpdateLower(int index)
{
    int curIndex = index;
    int lChild = index * 2 + 1;
    int rChild = lChild + 1;
    int minNode = curIndex;

    while (true)
    {
        if (lChild < openNodes.size() && openNodes[lChild]->GetCost() < openNodes[minNode]->GetCost())
            minNode = lChild;

        if (rChild < openNodes.size() && openNodes[rChild]->GetCost() < openNodes[minNode]->GetCost())
            minNode = rChild;

        if (minNode == curIndex)
            return;

        Swap(curIndex, minNode);
        curIndex = minNode;
        lChild = curIndex * 2 + 1;
        rChild = lChild + 1;
    }
}

void Heap::Swap(int n1, int n2)
{
    swap(openNodes[n1], openNodes[n2]);
}
