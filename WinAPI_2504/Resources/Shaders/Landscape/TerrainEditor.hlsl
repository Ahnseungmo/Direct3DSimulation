#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

LightPixelInput VS(VertexUVNormalTangent input)
{
    LightPixelInput output;
    output.pos = mul(input.pos, world);
    
    output.worldPos = output.pos.xyz;
    output.viewPos = invView._41_42_43;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
    
    output.uv = input.uv;
    
    return output;
}

cbuffer BrushBuffer : register(b10)
{
    int type;
    float3 pickingPos;
	
    float range;
    float3 color;
}

float4 BrushColor(float3 pos)
{
    float2 direction = pos.xz - pickingPos.xz;
	
    float dist = length(direction);
	
    if (dist < range)
        return float4(color, 0);

    return float4(0, 0, 0, 0);
}

Texture2D secondMap : register(t11);

float4 PS(LightPixelInput input) : SV_TARGET
{
    Material material = GetMaterial(input);
    
    float4 second = secondMap.Sample(samplerState, input.uv);
    
    material.diffuseColor = lerp(material.diffuseColor, second, input.tangent.x);
    
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
        //else if (lights[i].type == 3)
        //    color += CalcCapsule(material, lights[i]);
    }
    
    return ambient + color + emissive + BrushColor(material.worldPos);
}