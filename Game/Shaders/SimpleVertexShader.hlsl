cbuffer FrameConstantBuffer : register(b0)
{
    matrix view;
    matrix projection;
};

cbuffer EntityConstantBuffer : register(b1)
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
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    float4 pos = float4(input.pos, 1.0f);
    output.pos = mul(pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.normal = normalize(mul(input.normal, world));
    output.normal = float3(0.0f, 0.0f, 0.0f);
    output.texCoord = input.texCoord;
    
    return output;
}