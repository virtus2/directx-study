Texture2D diffuse : register(t0);
SamplerState diffuseSampler : register(s0);

cbuffer LightPixelConstantBuffer : register(b0)
{
    float3 cameraPosition;
    float3 lightPosition;
    float3 lightDirection;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 posWorld : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

float4 Gooch(PS_INPUT input)
{
    // REAL-TIME RENDERING 4/e. 175p
    float3 surfaceColor = diffuse.Sample(diffuseSampler, input.texCoord).xyz;
    float3 toEye = normalize(cameraPosition - input.posWorld);
    float3 toLight = normalize(lightPosition - input.posWorld);
    
    float3 cool = float3(0.0, 0.0, 0.55f) + 0.5f * surfaceColor;
    float3 warm = float3(0.3f, 0.3f, 0.0) + 0.5f * surfaceColor;
    float3 highlight = float3(1.0, 1.0, 1.0);
    
    float ndotl = saturate(dot(input.normal, toLight));
    float t = 0.5 * (1.0 + ndotl);
    float r = 2.0 * ndotl * input.normal - toLight;
    float s = saturate(100.0 * (dot(r, toEye) - 0.97));

    float3 shadedColor = s * highlight + (1.0 - s) * (t * warm + (1.0 - t) * cool);
    return float4(shadedColor, 1.0);
}

float4 SimpleLight(PS_INPUT input)
{
    float3 toLight = normalize(lightPosition - input.posWorld);
    float ndotl = normalize(dot(input.normal, toLight));
    float3 surfaceColor = diffuse.Sample(diffuseSampler, input.texCoord).xyz;
    
    return float4(surfaceColor * ndotl, 1.0);
}

float4 main(PS_INPUT input) : SV_TARGET
{
    return SimpleLight(input);
}