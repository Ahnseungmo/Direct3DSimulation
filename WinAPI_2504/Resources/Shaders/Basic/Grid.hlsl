//Shader : GPU를 다루는 프로그래밍 언어
//시멘틱(Sementic) : 반환자료형이나 매개변수의 사용용도를 표기하는 역할
//SV : System Value

//Constant Buffer(상수버퍼) : CPU에서 전달하는 값
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
    float4 color : COLOR;
};

struct Output
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

Output VS( Input input )
{
    Output output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.color = input.color;
    
	return output;
}

float4 PS( Output output ) : SV_TARGET
{
    return output.color;
}