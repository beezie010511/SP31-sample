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

[maxvertexcount(37)]
void main(
    triangle GS_IN input[3],
	inout TriangleStream<GS_OUT> output
)
{
    GS_OUT vtx;

    for (uint j = 0; j < 3; j++)
    {
        for (uint i = 0; i < 3; i++)
        {
            //頂点の情報をコピー
            vtx.pos = input[i].pos;
            vtx.uv = input[i].uv;
            vtx.normal = input[i].normal;
            vtx.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
            //法線方向にずらしていってシェルを構築する
            vtx.pos.xyz += vtx.normal * 0.001f * j;
            vtx.pos.y -= 0.0009f * j; //重力で毛先が下がるだろう
            //根元が暗くなるように色を調整
            vtx.color.rgb = (j / (10.0f - 1.0f)) * 0.5f + 0.5f;
            vtx.color.rgb = 1.0f - pow(1.0f - vtx.color.rgb, 2.0f);
            //αを利用して1層目(表皮の表示フラグを設定)
            vtx.color.a = (j == 0 ? 0.0f : 1.0f);
            
            //行列で変換
            vtx.pos = mul(vtx.pos, world);
            vtx.pos = mul(vtx.pos, view);
            vtx.pos = mul(vtx.pos, proj);
            
            output.Append(vtx);
        }
        output.RestartStrip();
    }

    GS_OUT finVtx;

    //---視角の計算
    //一つ目の頂点を設定
    finVtx.pos = input[0].pos;
    finVtx.uv = input[0].uv;
    finVtx.normal = input[0].normal;
    // Alpha values above 2 select the fin mask in PS_Fur.
    finVtx.color = float4(0.7f, 0.7f, 0.7f, 3.0f);
    finVtx.pos = mul(finVtx.pos, world);
    finVtx.pos = mul(finVtx.pos, view);
    finVtx.pos = mul(finVtx.pos, proj);
    output.Append(finVtx);
    
    //二つ目の頂点を設定(入力の二つの頂点の中間値)
    GS_OUT midVtx;
    midVtx.pos = (input[1].pos + input[2].pos) / 2.0f;
    midVtx.uv = (input[1].uv + input[2].uv) / 2.0f;
    midVtx.normal = (input[1].normal + input[2].normal) / 2.0f;
    midVtx.color = float4(0.7f, 0.7f, 0.7f, 1.0f);
    finVtx = midVtx;
    finVtx.pos = mul(finVtx.pos, world);
    finVtx.pos = mul(finVtx.pos, view);
    finVtx.pos = mul(finVtx.pos, proj);
    output.Append(finVtx);
    
    //三つ目の頂点の設定(一つ目の上
    finVtx.pos = input[0].pos;
    finVtx.uv = input[0].uv;
    finVtx.normal = input[0].normal;
    finVtx.color = float4(1.0f, 1.0f, 1.0f, 3.0f);
    finVtx.pos.xyz += finVtx.normal * 0.009f;
    finVtx.pos = mul(finVtx.pos, world);
    finVtx.pos = mul(finVtx.pos, view);
    finVtx.pos = mul(finVtx.pos, proj);
    output.Append(finVtx);

    //四つ目の頂点を設定(二つ目の上
    finVtx = midVtx;
    finVtx.color.a = 3.0f;
    finVtx.pos.xyz += finVtx.normal * 0.009f;
    finVtx.pos = mul(finVtx.pos, world);
    finVtx.pos = mul(finVtx.pos, view);
    finVtx.pos = mul(finVtx.pos, proj);
    output.Append(finVtx);
    
    output.RestartStrip();
}