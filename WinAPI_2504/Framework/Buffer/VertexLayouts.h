#pragma once

struct Vertex//정점 : 3차원 공간에서의 한 점
{
    float x, y, z;    

    Vertex(float x = 0, float y = 0, float z = 0)
        : x(x), y(y), z(z)
    {
    }
};

struct VertexColor//정점 : 3차원 공간에서의 한 점
{
    Float3 pos;
    Float4 color;

	VertexColor() : pos(0, 0, 0), color(1, 1, 1, 1)
	{
	}
	VertexColor(float x, float y, float z, float r, float g, float b, float a = 1.0f)
		: pos(x, y, z), color(r, g, b, a)
	{
	}
};

struct VertexUV//정점 : 3차원 공간에서의 한 점
{
    float x, y, z;
	float u, v; // Texture coordinates

    VertexUV(float x = 0, float y = 0, float z = 0, float u = 0, float v = 0)
		: x(x), y(y), z(z), u(u), v(v)
    {
    }
    VertexUV(Vector3 pos, Vector2 uv)
        : x(pos.x), y(pos.y), z(pos.z), u(uv.x), v(uv.y)
    {
    }
};

struct VertexUVNormal
{
    Float3 pos = {};
    Float2 uv = {};
    Float3 normal = {};

    VertexUVNormal()		
    {
    }

    VertexUVNormal(float x, float y, float z, float u, float v)
        : pos(x, y, z), uv(u, v)
    {
    }
};

struct VertexUVNormalTangent
{
    Float3 pos = {};
    Float2 uv = {};
    Float3 normal = {};
	Float3 tangent = {};

    VertexUVNormalTangent()
    {
    }

    VertexUVNormalTangent(float x, float y, float z, float u, float v)
        : pos(x, y, z), uv(u, v)
    {
    }
};

struct VertexUVNormalAlpha
{
    Float3 pos = {};
    Float2 uv = {};
    Float3 normal = {};
    float alpha[3] = {};

    VertexUVNormalAlpha()
    {
    }
};

struct VertexUVNormalTangentBlend
{
    Float3 pos = {};
    Float2 uv = {};
    Float3 normal = {};
    Float3 tangent = {};
	Float4 indices = {};
	Float4 weights = {};

    VertexUVNormalTangentBlend()
    {
    }

    VertexUVNormalTangentBlend(float x, float y, float z, float u, float v)
        : pos(x, y, z), uv(u, v)
    {
    }
};

struct InstanceData
{
	Matrix world;
    int index = 0;
};