#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct Output
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
};

Output VS(VertexUVNormal input)
{
    Output output;
    output.pos = mul(input.pos, world);
    
    output.viewDir = output.pos.xyz - invView._41_42_43;
    
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
    float3 light = normalize(lightDirection);
    float3 viewDir = normalize(output.viewDir);
    
    float diffuseIntensity = saturate(dot(normal, -light));
    
    float4 specularColor = 0;
    if (diffuseIntensity > 0)
    {
        float3 reflectDir = reflect(light, normal);
        
        specularColor = saturate(dot(viewDir, -reflectDir));
        float specularIntencity = specularMap.Sample(samplerState, output.uv);
        
        specularColor = specularIntencity * pow(specularColor, shininess) * mSpecular;
    }    
    
    float4 diffuseColor = baseColor * diffuseIntensity * mDiffuse;
    float4 ambientColor = baseColor * ambient * mAmbient;    
    
    return diffuseColor + specularColor + ambientColor;
}