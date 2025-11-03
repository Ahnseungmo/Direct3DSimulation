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
    float2 uv : UV;
};

struct Output
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

Output VS(Input input)
{
    Output output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    return output;
}

cbuffer ColorBuffer : register(b0)
{
    float4 color;
}

cbuffer ValueBuffer : register(b1)
{
    float value;
}

Texture2D baseMap : register(t0);
Texture2D secondMap : register(t1);
SamplerState samplerState : register(s0);

float4 PS(Output output) : SV_TARGET
{
    float4 baseColor = baseMap.Sample(samplerState, output.uv);
    
    float uv = output.uv;
    uv.x += value;
    
    float4 secondColor = secondMap.Sample(samplerState, uv);
    
    float4 resultColor = baseColor * secondColor;
    resultColor.a = baseColor.a;
    
    return resultColor;
}