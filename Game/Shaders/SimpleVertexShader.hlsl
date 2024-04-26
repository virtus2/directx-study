cbuffer FrameVertexConstantBuffer : register(b0)
{
    Matrix viewMatrix;
    Matrix projMatrix;

    float3 worldPosition;
    float focalLength;
    float3 targetPosition;
    float _padding1;
    float3 up;
    float _padding2;
};

cbuffer ObjectVertexConstantBuffer : register(b1)
{
    matrix world;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 posWorld : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4 pos = float4(input.pos, 1.0f);
    pos = mul(pos, world);
    output.posWorld = pos.xyz;
    pos = mul(pos, viewMatrix);
    pos = mul(pos, projMatrix);
    output.pos = pos;
    
    output.normal = mul(input.normal, (float3x3)world);
    output.texCoord = input.texCoord;
    
    return output;
}