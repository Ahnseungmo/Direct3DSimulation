//Constants
#define MAX_LIGHT 16

//Struct
struct Material
{
    float3 normal;
    float4 diffuseColor;
    float4 specularIntensity;
    float3 viewPos;
    float3 worldPos;
};

struct LightPixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 normal : NORMAL;
    float3 viewPos : POSITION0;
    float3 worldPos : POSITION1;
};

struct Light
{
    float4 color;
    
    float3 direction;
    int type;
    
    float3 position;
    float range;
    
    float inner;
    float outer;
    float length;
    int active;
};

//Constant Buffer
cbuffer MateiralBuffer : register(b0)
{
    float4 mDiffuse;
    float4 mSpecular;
    float4 mAmbient;
    float4 mEmissive;
    
    float shininess;
    int hasNormalMap;
}

cbuffer LightBuffer : register(b1)
{    
    Light lights[MAX_LIGHT];
    
    uint lightCount;
    float3 ambientLight;
    
    float3 ambientCeil;
}

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

SamplerState samplerState : register(s0);

//Function
float3 CalcNormal(float3 T, float3 B, float3 N, float2 uv)
{
    T = normalize(T);
    B = normalize(B);
    N = normalize(N);
    
    //[brunch]
    [flatten]
    if (hasNormalMap)
    {
        float3 normal = normalMap.Sample(samplerState, uv).rgb;
        normal = normal * 2.0f - 1.0f; //0~1 -> -1~1
        float3x3 TBN = float3x3(T, B, N);
        N = normalize(mul(normal, TBN));        
    }
    
    return N;
}

Material GetMaterial(LightPixelInput input)
{
    Material material;
    
    material.diffuseColor = diffuseMap.Sample(samplerState, input.uv);
    material.specularIntensity = specularMap.Sample(samplerState, input.uv);    
    material.normal = CalcNormal(input.tangent, input.binormal, input.normal, input.uv);    
    material.viewPos = input.viewPos;
    material.worldPos = input.worldPos;
    
    return material;
}

float4 CalcAmbient(Material material)
{
    float up = material.normal.y * 0.5f + 0.5f;
    
    float4 ambient = float4(ambientLight + ambientCeil * up, 1.0f);
    
    return ambient * material.diffuseColor * mAmbient;
}

float4 CalcFresnel(Material material)
{
    float emissiveIntensity = 0.0f;
    
    [flatten]
    if(mEmissive.a > 0.0f)
    {
        float3 viewDir = normalize(material.viewPos - material.worldPos);
        
        float t = saturate(dot(material.normal, viewDir));
        emissiveIntensity = smoothstep(1.0f - mEmissive.a, 1.0f, 1.0f - t);
    }
    
    return mEmissive * emissiveIntensity;
}

float4 CalcDirectional(Material material, Light light)
{
    float3 toLight = normalize(light.direction);
    
    float diffuseIntensity = saturate(dot(material.normal, -toLight));
    float4 finalColor = light.color * diffuseIntensity * mDiffuse;
    
    [flatten]
    if (diffuseIntensity > 0)
    {
        //Phong Shading
        //float3 reflectDir = reflect(toLight, material.normal);
        //float3 toView = normalize(material.worldPos - material.viewPos);        
        //float specularIntensity = saturate(dot(toView, -reflectDir));
        
        //Blinn-Phong Shading
        float3 toView = normalize(material.worldPos - material.viewPos);
        float3 halfWay = normalize(toView + toLight);
        float specularIntensity = saturate(dot(material.normal, -halfWay));
        
        finalColor += light.color * material.specularIntensity *
            pow(specularIntensity, shininess) * mSpecular;        
    }
    
    return finalColor * material.diffuseColor;
}

float4 CalcPoint(Material material, Light light)
{
    float3 toLight = material.worldPos - light.position;
    float distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float diffuseIntensity = saturate(dot(material.normal, -toLight));
    float4 finalColor = light.color * diffuseIntensity * mDiffuse;
    
    [flatten]
    if (diffuseIntensity > 0)
    {
        float3 toView = normalize(material.worldPos - material.viewPos);
        float3 halfWay = normalize(toView + toLight);
        float specularIntensity = saturate(dot(material.normal, -halfWay));
        
        finalColor += light.color * material.specularIntensity *
            pow(specularIntensity, shininess) * mSpecular;
    }
    
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attenuation = pow(distanceToLightNormal, 2.0f);
    
    return finalColor * material.diffuseColor * attenuation;
}

float4 CalcSpot(Material material, Light light)
{
    float3 toLight = material.worldPos - light.position;
    float distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float diffuseIntensity = saturate(dot(material.normal, -toLight));
    float4 finalColor = light.color * diffuseIntensity * mDiffuse;
    
    [flatten]
    if (diffuseIntensity > 0)
    {
        float3 toView = normalize(material.worldPos - material.viewPos);
        float3 halfWay = normalize(toView + toLight);
        float specularIntensity = saturate(dot(material.normal, -halfWay));
        
        finalColor += light.color * material.specularIntensity *
            pow(specularIntensity, shininess) * mSpecular;
    }
    
    float3 dir = normalize(light.direction);
    float cosAngle = dot(toLight, dir);
    
    float outer = cos(radians(light.outer));
    float inner = 1.0f / cos(radians(light.inner));
    
    float coneAttenuation = saturate((cosAngle - outer) * inner);
    coneAttenuation = pow(coneAttenuation, light.length);    
    
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attenuation = pow(distanceToLightNormal, light.length);
    
    return finalColor * material.diffuseColor * attenuation * coneAttenuation;
}
float4 CalcGlobal(Material material, Light light)
{

    float3 N = normalize(material.normal);
    
    // Global Light 방향 (씬 전체에서 비추는 가상 방향)
    float3 L = normalize(-light.direction);

    // Lambert Diffuse
    float NdotL = max(dot(N, L), 0.0f);

    // Ambient 색상 : 전체 씬의 기본 밝기
    float3 ambient = material.diffuseColor.rgb * 0.5; // 0.5 정도면 천장/벽 밝기 적절

    // Diffuse : 방향성 있는 Global 라이트
    float3 diffuse = material.diffuseColor.rgb * light.color.rgb * NdotL * 0.5;
    // 0.5로 줄이면 과도하게 밝지 않고 입체감 유지

    // Specular
    float3 viewDir = float3(0, 0, 1);
    float3 reflectDir = reflect(-L, N);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    float3 specular = material.specularIntensity.rgb * light.color.rgb * spec;

    float3 finalColor = ambient + diffuse + specular;

    return float4(finalColor, 1.0f);

}
//float4 CalcCapsule(Material material, Light light)
//{
//    float3 direction = normalize(light.direction);
//    float3 start = material.worldPos - light.position;
//    float distanceOnLine = dot(start, direction) / light.length;
//    distanceOnLine = saturate(distanceOnLine) * light.length;
//    
//    float3 pointOnLine = light.position + direction * distanceOnLine;
//    
//    float3 toLight = material.worldPos - pointOnLine;
//    float distanceToLight = length(toLight);
//    toLight /= distanceToLight;
//    
//    float diffuseIntensity = saturate(dot(material.normal, -toLight));
//    float4 finalColor = light.color * diffuseIntensity * mDiffuse;
//    
//    [flatten]
//    if (diffuseIntensity > 0)
//    {
//        float3 toView = normalize(material.worldPos - material.viewPos);
//        float3 halfWay = normalize(toView + toLight);
//        float specularIntensity = saturate(dot(material.normal, -halfWay));
//        
//        finalColor += light.color * material.specularIntensity *
//            pow(specularIntensity, shininess) * mSpecular;
//    }
//    
//    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
//    float attenuation = pow(distanceToLightNormal, 2.0f);
//    
//    return finalColor * material.diffuseColor * attenuation;
//}