#include "Framework.h"

Node::Node(Vector2 pos, int index)
    : CircleCollider(10), index(index)
{
    localPosition = pos;
    UpdateWorld();
}

Node::~Node()
{
    for (Edge* edge : edges)
        delete edge;
}

void Node::Render()
{
    switch (state)
    {
    case Node::None:
        SetColor(0, 1, 1);
        break;
    case Node::Open:
        SetColor(0, 0, 1);
        break;
    case Node::Closed:
        SetColor(1, 0, 0);
        break;
    case Node::Using:
        SetColor(0, 1, 0);
        break;
    case Node::Obstacle:
        SetColor(0, 0, 0);
        break;
    }

    Collider::Render();

    for (Edge* edge : edges)
        edge->line->Render();
}

void Node::AddEdge(Node* node)
{
    Edge* edge = new Edge(node->GetLocalPosition(), localPosition);
    edge->index = node->index;
    edge->cost = Vector2::Distance(node->GetLocalPosition(), localPosition);

    edges.push_back(edge);
}
