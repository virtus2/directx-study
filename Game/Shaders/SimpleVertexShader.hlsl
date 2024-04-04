cbuffer BasicVertexConstantBuffer : register(b0)
{
    matrix model;
    matrix invTranspose;
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
    pos = mul(pos, model);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    
    output.pos = pos;
    
    float4 normal = float4(input.normal, 0.0f);
    output.normal = mul(normal, invTranspose).xyz;
    output.normal = normalize(output.normal);
    
    output.texCoord = input.texCoord;
    
    return output;
}