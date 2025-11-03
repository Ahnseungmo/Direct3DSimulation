cbuffer WorldBuffer : register(b0)
{
    matrix world;
}
cbuffer ViewBuffer : register(b1)
{
    matrix view;
}
cbuffer ProjectionBuffer : register(b2)
{
    matrix projection;
}

struct Input
{
    float4 pos : POSITION;    
};

struct Output
{
    float4 pos : SV_POSITION;    
};

Output VS(Input input)
{
    Output output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);    
    
    return output;
}

cbuffer ColorBuffer : register(b0)
{
    float4 color;
}

float4 PS(Output output) : SV_TARGET
{
    return color;
}