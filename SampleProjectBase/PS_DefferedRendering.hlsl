static const float PI = 3.1415926f;

struct PS_IN
{
    float4 pos : SV_POSITION; // スクリーン座標
    float2 uv : TEXCOORD0; // テクスチャ座標
    float4 color : COLOR0; // アルベド(色)情報
};

//ディファードレンダリングにおいてGBufferに描き込まれた情報をもとに
//レンダリング計算を行うためのサンプラーおよびテクスチャ
SamplerState samp : register(s0);
Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D worldTex : register(t2);

//ライト情報を保持する定数バッファ
cbuffer Light : register(b0)
{
    float4 lightAmbient; //ライトの環境光
    float4 lightDiffuse; //ライトの拡散反射光
    float4 lightDir;     //ライトの方向
}

float4 main(PS_IN pin) : SV_TARGET
{
    //テクスチャからアルベド(色)情報をサンプリング
    float4 color = albedoTex.Sample(samp, pin.uv);
    
    //テクスチャから法線ベクトルをサンプリングし正規化
    float3 N = normalize(normalTex.Sample(samp, pin.uv).rgb);
    
    //テクスチャからワールド座標をサンプリング(使ってない)
    float3 worldPos = worldTex.Sample(samp, pin.uv).rgb;
    
    //ピクセルの1点に対して、全ての点光源との明るさを計算する
    float3 L = normalize(-lightDir.xyz); //ライトの方向を正規化
    float diffuse = saturate(dot(N, L)); //拡散反射光の計算(法線ベクトルとライトの方向の内積)
    
    color *= diffuse * lightDiffuse / PI + lightAmbient; // カラーに拡散反射光と環境光を適用
    
    //最終的な色を返す
    return color;
}