#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

LightPixelInput VS(VertexUVNormalTangentBlend input)
{
    LightPixelInput output;
    output.pos = mul(input.pos, world);
    output.worldPos = output.pos;
	
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
	
    return output;
}

float4 PS(LightPixelInput input) : SV_TARGET
{
    float depth = input.pos.z / input.pos.w;
	
    return float4(depth.xxx, 1);
}