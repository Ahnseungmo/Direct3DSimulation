#pragma once

class AStar
{
public:
    AStar(class TileMap* map);
    ~AStar();

    void Render();

    int FindCloseNode(const Vector2& pos);

    void GetPath(IN const int& start, IN const int& end, OUT vector<Vector2>& path);
    vector<Vector2> GetPath(const int& start, const int& end);


private:
    void Reset();

    float GetManhattanDistance(int start, int end);
    float GetDiagonalManhattanDistance(const int& start, const int& end);


    void Extend(const int& center, const int& end);
    int GetMinNode();

    void SetEdge();

private:
    vector<Node*> nodes;
    //vector<int> openNodes;
    Heap* heap;

    Vector2 tileSize;
    int cols;
};