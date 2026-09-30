struct PS_IN
{
    float4 pos : SV_POSITION0; // スクリーン座標
    float2 uv : TEXCOORD0; // テクスチャ座標
    float3 normal : NORMAL0; // 法線ベクトル
    float4 worldPos : POSITION0; // ワールド座標
};

//複数の出力を持つため構造体にまとめる(出力先はSV_TARGETxで指定)
struct PS_OUT
{
    float4 albedo : SV_TARGET0; // アルベド(色)出力
    float4 normal : SV_TARGET1; // 法線ベクトル出力
    float4 world : SV_TARGET2;  // ワールド座標出力
};

SamplerState samp : register(s0);
Texture2D tex : register(t0);

PS_OUT main(PS_IN pin)
{
    PS_OUT pout;
    //アルベド（色）をテクスチャからサンプリング
    pout.albedo = tex.Sample(samp, pin.uv);
    ////法線ベクトルを正規化成分を1に設定
    pout.normal = float4(normalize(pin.normal), 1.0);
    ////ワールド座標を出力
    pout.world = pin.worldPos;
    
    
    return pout;
}