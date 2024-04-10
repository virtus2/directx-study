cbuffer SimpleVertexConstantBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    float4 pos = float4(input.pos, 1.0f);
    output.pos = mul(pos, model);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    float4 normal = float4(input.normal, 0.0f);
    output.normal = normalize(mul(normal, model));
    output.texCoord = input.texCoord;
    
    return output;
}