struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
};

cbuffer Light : register(b0)
{
    float4 lightDiffuse;
    float4 lightAmbient;
    float4 lightDir;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    color = tex.Sample(samp, pin.uv);

    float3 N = normalize(pin.normal);
    float3 L = normalize(-lightDir.xyz);
    float diffuse = saturate(dot(N, L));
    
    color *= diffuse * lightDiffuse / 3.1415926f + lightAmbient;
    
    return color;
}