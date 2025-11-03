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

cbuffer FilterBuffer : register(b1)
{
    float filterType; // 0 for Grayscale, 1 for Sepia, etc.
    float filterValue;
    
    float2 imageSize;
}

cbuffer RadialBlurBuffer : register(b2)
{
    float3 radialScale;    
}

float4 Grayscale(float4 color)
{
    float scale = (color.r + color.g + color.b) / 3;
    
    //return float4(scale, scale, scale, color.a);
    return float4(scale.xxx, color.a);
}

float4 Grayscale2(float4 color)
{
    //float3 grayColor;
    //grayColor.r = color.r * 0.3f;
    //grayColor.g = color.g * 0.59f;
    //grayColor.b = color.b * 0.11f;
    //
    //float scale = grayColor.r + grayColor.g + grayColor.b;
    
    float scale = dot(color.rgb, float3(0.3f, 0.59f, 0.11f));
    
    return float4(scale.xxx, color.a);
}

float4 Sepia(float4 color)
{
    float3 sepiaColor;
    sepiaColor.r = dot(color.rgb, float3(0.393f, 0.769f, 0.189f));
    sepiaColor.g = dot(color.rgb, float3(0.349f, 0.686f, 0.168f));
    sepiaColor.b = dot(color.rgb, float3(0.272f, 0.534f, 0.131f));
    
    return float4(sepiaColor, color.a);
}

float4 Mosaic(float2 uv)
{
    float2 temp = floor(uv * filterValue) / filterValue;
    
    return baseMap.Sample(samplerState, temp);
}

float4 Blur(float2 uv)
{
    float4 result = 0;
    
    for (int i = 0; i < filterValue; i++)
    {
        float2 div = (1 + i) / imageSize;
        
        result += baseMap.Sample(samplerState, float2(uv.x + div.x, uv.y));
        result += baseMap.Sample(samplerState, float2(uv.x - div.x, uv.y));
        result += baseMap.Sample(samplerState, float2(uv.x, uv.y + div.y));
        result += baseMap.Sample(samplerState, float2(uv.x, uv.y - div.y));
    }
    
    return result /= filterValue * 4.0f;
}

static const float2 edges[8] =
{
    float2(-1, -1), float2(0, -1), float2(+1, -1),
	float2(-1, 0), float2(+1, 0),
	float2(-1, +1), float2(0, +1), float2(+1, +1),
};

float4 Blur2(float2 uv)
{
    float4 result = 0;
	
    for (int i = 0; i < filterValue; i++)
    {
        float2 div = (1 + i) / (float2) imageSize;
		
        for (int j = 0; j < 8; j++)
        {
            float2 xy = edges[j] * div + uv;
			
            result += baseMap.Sample(samplerState, xy);
        }
    }
	
    return result /= filterValue * 8;
}

float4 RadialBlur(float2 uv)
{
    float2 radiusUV = uv - float2(0.5f, 0.5f);
    float r = length(radiusUV);
    radiusUV /= r;
	
    r = saturate(r / radialScale.y);
	
    float2 delta = -radiusUV * r * r * radialScale.z / radialScale.x;
	
    float4 result = 0;
	
    //[unroll(8)]
    for (int i = 0; i < radialScale.x; i++)
    {
        uv += delta;
        result += baseMap.Sample(samplerState, uv);
    }
	
    return result /= radialScale.x;
}

static const float weights[13] =
{
    0.0561f, 0.1353f, 0.2730f, 0.4868f, 0.7261f, 0.9231f,
	1.0f,
	0.9231f, 0.7261f, 0.4868f, 0.2730f, 0.1353f, 0.0561f
};

float4 GaussianBlur(float2 uv)
{
    float2 div = 1.0f / imageSize;
	
    float sum = 0.0f;
    float4 result = 0;
	
    for (int i = -6; i <= 6; i++)
    {
        float2 temp = uv + float2(div.x * i, 0);
        result += weights[6 + i] * baseMap.Sample(samplerState, temp);
		
        temp = uv + float2(0, div.y * i);
        result += weights[6 + i] * baseMap.Sample(samplerState, temp);

        sum += weights[6 + i] * 2.0f;
    }
	
    result /= sum;
	
    return result;
}

float4 PS(Output output) : SV_TARGET
{
    float4 baseColor = baseMap.Sample(samplerState, output.uv);
    
    //[flatten]
    [branch]    
    if(filterType == 1)
        return Grayscale(baseColor);
    else if (filterType == 2)
        return Grayscale2(baseColor);
    else if (filterType == 3)
        return Sepia(baseColor);
    else if (filterType == 4)
        return Mosaic(output.uv);
    else if (filterType == 5)
        return Blur(output.uv);
    else if (filterType == 6)
        return Blur2(output.uv);
    else if (filterType == 7)
        return RadialBlur(output.uv);
    else if (filterType == 8)
        return GaussianBlur(output.uv);
    
    return baseColor * color;
}