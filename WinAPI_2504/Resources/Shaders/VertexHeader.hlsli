#define MAX_INSTANCE 128

cbuffer WorldBuffer : register(b0)
{
    matrix world;
}
cbuffer ViewBuffer : register(b1)
{
    matrix view;
    matrix invView;
}
cbuffer ProjectionBuffer : register(b2)
{
    matrix projection;
}

struct Frame
{
    int clip;
    int curFrame;
    float time;
    float scale;
};

struct Motion
{
    float duration;
    float tweenTime;
    float runningTime;
    float playTime;
    
    Frame cur, next;
};

cbuffer FrameBuffer : register(b3)
{
    Motion motion;
}

cbuffer FrameBuffer : register(b4)
{
    Motion motions[MAX_INSTANCE];
}

Texture2DArray transformMap : register(t0);

//Functions

matrix LoadTransform(int index, int frame, int clip)
{
    float4 f0 = transformMap.Load(int4(index * 4 + 0, frame, clip, 0));
    float4 f1 = transformMap.Load(int4(index * 4 + 1, frame, clip, 0));
    float4 f2 = transformMap.Load(int4(index * 4 + 2, frame, clip, 0));
    float4 f3 = transformMap.Load(int4(index * 4 + 3, frame, clip, 0));
	
    return matrix(f0, f1, f2, f3);
}

matrix SkinWorld(float4 indices, float4 weights)
{
    matrix transform = 0;
    matrix curAnim, nextAnim;
    
    [unroll(4)]
    for (int i = 0; i < 4; i++)
    {
        if(weights[i] == 0.0f)
            continue;
        
        int clip = motion.cur.clip;
        int curFrame = motion.cur.curFrame;
        
        matrix cur = LoadTransform(indices[i], curFrame, clip);
        matrix next = LoadTransform(indices[i], curFrame + 1, clip);
        
        curAnim = lerp(cur, next, motion.cur.time);
        
        if(motion.next.clip > -1)
        {
            clip = motion.next.clip;
            curFrame = motion.next.curFrame;
            
            matrix cur = LoadTransform(indices[i], curFrame, clip);
            matrix next = LoadTransform(indices[i], curFrame + 1, clip);
        
            nextAnim = lerp(cur, next, motion.next.time);
            curAnim = lerp(curAnim, nextAnim, motion.tweenTime);
        }
        
        transform += weights[i] * curAnim;
    }

    return transform;
}

matrix SkinWorld(int instanceIndex, float4 indices, float4 weights)
{
    matrix transform = 0;
    matrix curAnim, nextAnim;
    
    [unroll(4)]
    for (int i = 0; i < 4; i++)
    {
        if (weights[i] == 0.0f)
            continue;
        
        Motion motion = motions[instanceIndex];
        
        int clip = motion.cur.clip;
        int curFrame = motion.cur.curFrame;
        
        matrix cur = LoadTransform(indices[i], curFrame, clip);
        matrix next = LoadTransform(indices[i], curFrame + 1, clip);
        
        curAnim = lerp(cur, next, motion.cur.time);
        
        if (motion.next.clip > -1)
        {
            clip = motion.next.clip;
            curFrame = motion.next.curFrame;
            
            matrix cur = LoadTransform(indices[i], curFrame, clip);
            matrix next = LoadTransform(indices[i], curFrame + 1, clip);
        
            nextAnim = lerp(cur, next, motion.next.time);
            curAnim = lerp(curAnim, nextAnim, motion.tweenTime);
        }
        
        transform += weights[i] * curAnim;
    }

    return transform;
}


//VertexLayouts

struct Vertex
{
    float4 pos : POSITION;
};

struct VertexUV
{
    float4 pos : POSITION;
    float2 uv : UV;
};

struct VertexColor
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct VertexUVNormal
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
};

struct VertexUVNormalTangent
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VertexUVNormalTangentBlend
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 indices : INDICES;
    float4 weights : WEIGHTS;
};

