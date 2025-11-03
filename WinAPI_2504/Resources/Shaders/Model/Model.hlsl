#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

LightPixelInput VS(VertexUVNormalTangentBlend input)
{
    LightPixelInput output;
    matrix transform = mul(SkinWorld(input.indices, input.weights), world);
    output.pos = mul(input.pos, transform);
    
    output.worldPos = output.pos.xyz;
    output.viewPos = invView._41_42_43;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.normal = mul(input.normal, (float3x3) transform);
    output.tangent = mul(input.tangent, (float3x3) transform);
    output.binormal = cross(output.normal, output.tangent);
    
    output.uv = input.uv;
    
    return output;
}

float4 PS(LightPixelInput input) : SV_TARGET
{
    Material material = GetMaterial(input);
    
    float4 ambient = CalcAmbient(material);
    float4 emissive = CalcFresnel(material);
    float4 color = float4(0, 0, 0, 0);
    
    [unroll(MAX_LIGHT)]
    for (int i = 0; i < lightCount; i++)
    {
        [flatten]
        if (!lights[i].active)
            continue;
        
        [flatten]
        if (lights[i].type == 0)
            color += CalcDirectional(material, lights[i]);
        else if (lights[i].type == 1)
            color += CalcPoint(material, lights[i]);
        else if (lights[i].type == 2)
            color += CalcSpot(material, lights[i]);
    }
    
    return ambient + color + emissive;
}