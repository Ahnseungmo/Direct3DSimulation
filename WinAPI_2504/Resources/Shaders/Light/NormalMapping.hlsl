#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct Output
{
    float4 pos : SV_POSITION;
    float2 uv : UV;    
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
};

Output VS(VertexUVNormalTangent input)
{
    Output output;
    output.pos = mul(input.pos, world);
    
    output.viewDir = output.pos.xyz - invView._41_42_43;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
    
    output.uv = input.uv;
    
    return output;
}

float4 PS(Output output) : SV_TARGET
{
    float4 baseColor = diffuseMap.Sample(samplerState, output.uv);
    
    float3 T = normalize(output.tangent);
    float3 B = normalize(output.binormal);
    float3 N = normalize(output.normal);
    
    float3 normal = N;
    
    float3 light = normalize(lightDirection);
    float3 viewDir = normalize(output.viewDir);
    
    if(hasNormalMap)
    {
        float3 normalMapColor = normalMap.Sample(samplerState, output.uv).rgb;
        normal = normalMapColor * 2.0f - 1.0f; //0~1 -> -1~1
        float3x3 TBN = float3x3(T, B, N);
        normal = normalize(mul(normal, TBN));
    }    
    
    float diffuseIntensity = saturate(dot(normal, -light));
    
    float4 specularColor = 0;
    if (diffuseIntensity > 0)
    {
        float3 reflectDir = reflect(light, normal);
        
        specularColor = saturate(dot(viewDir, -reflectDir));
        float specularIntencity = specularMap.Sample(samplerState, output.uv);
        
        specularColor = specularIntencity * pow(specularColor, shininess) * mSpecular * lightColor;
    }
    
    float4 diffuseColor = baseColor * diffuseIntensity * mDiffuse * lightColor;
    float4 ambientColor = baseColor * ambient * mAmbient;
    
    return diffuseColor + specularColor + ambientColor;
}