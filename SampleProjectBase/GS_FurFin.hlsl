struct GS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct GS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
};

cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

//ジオメトリシェーダーで生成する頂点の最大数
[maxvertexcount(7)]
void main(
 /* ジオメトリシェーダーの入力
point ...点、入力の数は１つだけ
line ...線、入力の数は２つ
triangle...三角形、入力の数は３つ
 */
triangle GS_IN input[3],
inout TriangleStream<GS_OUT> output
)
{
    GS_OUT vtx;
    
    //もとになる三角形をそのまま表示
    for (uint i = 0; i < 3; i++)
    {
        //頂点の情報をコピー
        vtx.pos = input[i].pos;
        vtx.uv = input[i].uv;
        vtx.normal = input[i].normal;
        vtx.color = float4(0.7f, 0.7f, 0.7f, 0.7f);
        
        //行列で変換
        vtx.pos = mul(vtx.pos, world);
        vtx.pos = mul(vtx.pos, view);
        vtx.pos = mul(vtx.pos, proj);
        
        //出力ストリームに頂点を追加
        output.Append(vtx);
    }
    //ここまで設定されている頂点の情報でポリゴンを表示(TRIANGLESTRIP))
    output.RestartStrip();
    
    //---視角の計算
    //一つ目の頂点を設定
    vtx.pos = input[0].pos;
    vtx.uv = input[0].uv;
    vtx.normal = input[0].normal;
    vtx.color = float4(0.7f, 0.7f, 0.7f, 1.0f);
    vtx.pos = mul(vtx.pos, world);
    vtx.pos = mul(vtx.pos, view);
    vtx.pos = mul(vtx.pos, proj);
    output.Append(vtx);
    
    //二つ目の頂点を設定(入力の二つの頂点の中間値)
    GS_OUT midVtx;
    midVtx.pos = (input[1].pos + input[2].pos) / 2.0f;
    midVtx.uv = (input[1].uv + input[2].uv) / 2.0f;
    midVtx.normal = (input[1].normal + input[2].normal) / 2.0f;
    midVtx.color = float4(0.7f, 0.7f, 0.7f, 1.0f);
    vtx = midVtx;
    vtx.pos = mul(vtx.pos, world);
    vtx.pos = mul(vtx.pos, view);
    vtx.pos = mul(vtx.pos, proj);
    output.Append(vtx);
    
    //三つ目の頂点の設定(一つ目の上
    vtx.pos = input[0].pos;
    vtx.uv = input[0].uv;
    vtx.normal = input[0].normal;
    vtx.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    vtx.pos.xyz += vtx.normal * 0.009f;
    vtx.pos = mul(vtx.pos, world);
    vtx.pos = mul(vtx.pos, view);
    vtx.pos = mul(vtx.pos, proj);
    output.Append(vtx);

    //四つ目の頂点を設定(二つ目の上
    vtx = midVtx;
    vtx.pos.xyz += vtx.normal * 0.009f;
    vtx.pos = mul(vtx.pos, world);
    vtx.pos = mul(vtx.pos, view);
    vtx.pos = mul(vtx.pos, proj);
    output.Append(vtx);
    
    output.RestartStrip();
}