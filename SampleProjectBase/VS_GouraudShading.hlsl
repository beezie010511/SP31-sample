struct VS_IN
{
    float3 pos : POSITION0;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

struct VS_OUT
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

//
cbuffer Light : register(b1)
{
    float4 lightColor;
    float4 lightAmbient;
    float4 lightDir;
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, world);
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    
    float3 N = vin.normal;
    float3 L = lightDir.xyz;
    
    N = mul(N, (float3x3) world);
    L = -L;
    
    N = normalize(N);
    L = normalize(L);
    
    float diffuse = saturate(dot(N, L));
    
    vout.color = float4(diffuse * lightColor.rgb + lightAmbient.rgb, 1.0f);
    
    vout.uv = vin.uv;
    
    return vout;
}

