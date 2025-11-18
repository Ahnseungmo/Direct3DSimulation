#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct VertexInput
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 indices : BLENDINDICES;
    float4 weights : BLENDWEIGHTS;
	
    matrix transform : INSTANCE_TRANSFORM;
    int index : INSTANCE_INDEX;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 worldPos : POSITION0;
    float3 viewPos : POSITION1;
};

LightPixelInput VS(VertexInput input)
{
    LightPixelInput output;
    matrix transform = SkinWorld(input.index, input.indices, input.weights);
    transform = mul(transform, input.transform);
	
    output.pos = mul(input.pos, transform);
    output.worldPos = output.pos;
	
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
	
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3) transform);
    output.tangent = mul(input.tangent, (float3x3) transform);
    output.binormal = cross(output.normal, output.tangent);
    output.viewPos = invView._41_42_43;
	
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
        else if (lights[i].type == 3)
            color += CalcGlobal(material, lights[i]);
    }
    
    return ambient + color + emissive;
}