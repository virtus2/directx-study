Texture2D diffuse : register(t0);
SamplerState diffuseSampler : register(s0);

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return diffuse.Sample(diffuseSampler, input.texCoord);
}