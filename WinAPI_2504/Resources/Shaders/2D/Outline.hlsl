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

cbuffer OutlineBuffer : register(b1)
{
    float2 imageSize;
    float weight;
}

Texture2D baseMap : register(t0);
SamplerState samplerState : register(s0);

float4 PS(Output output) : SV_TARGET
{
    float4 baseColor = baseMap.Sample(samplerState, output.uv);
    
    [branch]
    if(baseColor.a > 0.0f)
        return baseColor;
    
    float count = 0;
    
    for (int y = -1; y <= 1; y++)
    {
        for (int x = -1; x <= 1; x++)
        {   
            float2 offset = (float2(x, y) / imageSize) * weight;
            count += baseMap.Sample(samplerState, output.uv + offset).a;            
        }
    }
    
    [branch]
    if(count > 0 && count < 9)
        return color;
    
    return baseColor;
}