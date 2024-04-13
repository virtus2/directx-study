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
    return float4(1.0, 1.0, 1.0, 1.0) * diffuse.Sample(diffuseSampler, input.texCoord);
}