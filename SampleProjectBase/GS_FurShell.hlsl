struct GS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct GS_OUT
{
    float4 pos : SV_POSITION0;
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
[maxvertexcount(30)]
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
    for (uint j = 0; j < 3; j++)
    {
        for (uint i = 0; i < 3; i++)
        {
            GS_OUT vtx;
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
        //ここまで設定されている頂点の情報でポリゴンを表示(TRIANGLESTRIP))
        output.RestartStrip();
    }
}

