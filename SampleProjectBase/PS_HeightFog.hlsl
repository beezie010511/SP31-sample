struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : POSITION0;
};
cbuffer Light : register(b0)
{
    float4 lightColor;
    float4 lightAmbient;
    float4 lightDir;
};

cbuffer FogParam : register(b1)
{
	float4 fogColor; // �t�H�O�̐F
	float2 distanceFog; // �����t�H�O�̏��(���g�p
	float fogStart;	// �t�H�O�̊J�n����
	float fogRange; // �t�H�O�͈̔�
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	// �t�H�O�̐F��v�Z����n�_�܂ł̍���
	float height = pin.worldPos.y;

	// �t�H�O�̐F��ω�������W���̌v�Z
	float fog = 0.0f;
	fog = height - fogStart; // �t�H�O�̊J�n�����ň���
	fog = fog / fogRange; // �t�H�O�̓K�p�͈͂Ŋ���
	fog = saturate(-fog); // �}�C�i�X��|���Ă���A0�`1�Ŋۂߍ���

	// ���̐F
	float3 N = normalize(pin.normal);
	float3 L = normalize(lightDir.xyz);
	float diffuse = saturate(dot(N, -L));
	color = tex.Sample(samp, pin.uv);
	color.rgb = color.rgb * (diffuse + lightAmbient.rgb);

	// �W�����猳�̐F�ƃt�H�O�̐F��ω�������
	color.rgb = lerp(color.rgb, fogColor.rgb, fog);
	
	return color;
}