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
    float4 wPos : TEXCOORD1;
};

cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

[maxvertexcount(16)]
void main(
    triangle GS_IN input[3],
    inout TriangleStream<GS_OUT> output
)
{
    GS_OUT vtx;

    for (uint layer = 0; layer < 3; layer++)
    {
        for (uint i = 0; i < 3; i++)
        {
            vtx.pos = input[i].pos;
            vtx.uv = input[i].uv;
            vtx.normal = input[i].normal;
            vtx.color = float4(1.0f, 1.0f, 1.0f, layer == 0 ? 0.0f : 1.0f);
            vtx.pos.xyz += vtx.normal * 0.001f * layer;
            vtx.pos.y -= 0.0009f * layer;
            vtx.color.rgb = (layer / 9.0f) * 0.5f + 0.5f;
            vtx.color.rgb = 1.0f - pow(1.0f - vtx.color.rgb, 2.0f);
            vtx.wPos = mul(vtx.pos, world);
            vtx.pos = mul(vtx.wPos, view);
            vtx.pos = mul(vtx.pos, proj);
            output.Append(vtx);
        }
        output.RestartStrip();
    }

    for (uint i = 0; i < 3; i++)
    {
        vtx.pos = input[i].pos;
        vtx.uv = input[i].uv;
        vtx.normal = input[i].normal;
        vtx.color = float4(0.7f, 0.7f, 0.7f, 0.0f);
        vtx.wPos = mul(vtx.pos, world);
        vtx.pos = mul(vtx.wPos, view);
        vtx.pos = mul(vtx.pos, proj);
        output.Append(vtx);
    }
    output.RestartStrip();

    GS_OUT midVtx;
    midVtx.pos = (input[1].pos + input[2].pos) * 0.5f;
    midVtx.uv = (input[1].uv + input[2].uv) * 0.5f;
    midVtx.normal = normalize(input[1].normal + input[2].normal);
    midVtx.color = float4(0.7f, 0.7f, 0.7f, 3.0f);
    midVtx.wPos = mul(midVtx.pos, world);

    vtx.pos = input[0].pos;
    vtx.uv = input[0].uv;
    vtx.normal = input[0].normal;
    vtx.color = float4(0.7f, 0.7f, 0.7f, 3.0f);
    vtx.wPos = mul(vtx.pos, world);
    vtx.pos = mul(vtx.wPos, view);
    vtx.pos = mul(vtx.pos, proj);
    output.Append(vtx);

    vtx = midVtx;
    vtx.pos = mul(vtx.wPos, view);
    vtx.pos = mul(vtx.pos, proj);
    output.Append(vtx);

    vtx.pos = input[0].pos;
    vtx.uv = input[0].uv;
    vtx.normal = input[0].normal;
    vtx.color = float4(1.0f, 1.0f, 1.0f, 3.0f);
    vtx.pos.xyz += vtx.normal * 0.009f;
    vtx.wPos = mul(vtx.pos, world);
    vtx.pos = mul(vtx.wPos, view);
    vtx.pos = mul(vtx.pos, proj);
    output.Append(vtx);

    vtx = midVtx;
    vtx.pos.xyz += vtx.normal * 0.009f;
    vtx.wPos = mul(vtx.pos, world);
    vtx.pos = mul(vtx.wPos, view);
    vtx.pos = mul(vtx.pos, proj);
    output.Append(vtx);
    output.RestartStrip();
}
