#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct Output
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;    
};

Output VS(VertexUVNormal input)
{
    Output output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.normal = mul(input.normal, (float3x3) world);    
    
    output.uv = input.uv;
    
    return output;
}

float4 PS(Output output) : SV_TARGET
{
    float4 baseColor = diffuseMap.Sample(samplerState, output.uv);
    
    float3 normal = normalize(output.normal);
    float3 light = normalize(-lightDirection);
    
    float diffuse = saturate(dot(normal, light));
    
    float4 diffuseColor = baseColor * diffuse;
    float4 ambientColor = baseColor * ambient;
    
    return diffuseColor + ambientColor;
}