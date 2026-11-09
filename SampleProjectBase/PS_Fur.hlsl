struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float4 wPos : TEXCOORD1;
};

cbuffer Camera : register(b0)
{
    float4 camPos;
};

Texture2D tex : register(t0);
Texture2D furMask : register(t1);
Texture2D furMask2 : register(t2);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = tex.Sample(samp, pin.uv);
    
    //マスク画像を使って、断面図として表示するか判定
    if (pin.color.a > 0.5f)
    {
        float mask = 1.0f;
        if (pin.color.a > 1.5f)
        {
            //カメラのベクトルと法線から、交差具合を判定
            float3 camVec = normalize((camPos - pin.wPos).xyz);
            float3 N = normalize(pin.normal);
            float d = dot(camVec, N);
            if (pin.color.a > 2.0f)
                mask = furMask2.Sample(samp, pin.uv * 70.0f).r * (1.0f - d);
            else
                mask = furMask.Sample(samp, pin.uv * 70.0f).r * (d + 0.4f);
        }
        else
            mask = furMask.Sample(samp, pin.uv * 70.0f).r;
        if (mask < 0.5f)
            discard;
    }
    color.rgb *= pin.color.rgb;
    return color;
}