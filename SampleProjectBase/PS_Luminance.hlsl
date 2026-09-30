struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

SamplerState samp : register(s0);
Texture2D tex : register(t0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = tex.Sample(samp, pin.uv);
    
    float t = dot(color.xyz, float3(0.215f, 0.7154f, 0.0721f));
    
    clip(t - 1.0f);
    return color;
}