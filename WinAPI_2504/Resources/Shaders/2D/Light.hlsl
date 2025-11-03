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

Texture2D baseMap : register(t0);
SamplerState samplerState : register(s0);

cbuffer LightBuffer : register(b1)
{
    float2 imageSize;
    float2 lightPos;    
}

cbuffer RangeBuffer : register(b2)
{    
    float range;
    float power;
}

float4 PS(Output output) : SV_TARGET
{
    float4 baseColor = baseMap.Sample(samplerState, output.uv);
    
    float2 coord = output.uv * imageSize;
    
    float distanceToLight = length(lightPos - coord);
    
    float temp = 1.0f - saturate(distanceToLight / range);
    
    temp = pow(temp, power);
    
    return float4((baseColor * temp).rgb, baseColor.a);
}