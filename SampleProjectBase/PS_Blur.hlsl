struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

cbuffer Param : register(b0)
{
	float2 texSize; // �e�N�X�`���T�C�Y
	float2 blurDir; // �u���[����������
};

SamplerState samp : register(s0);
Texture2D tex : register(t0);

float4 main(PS_IN pin) : SV_TARGET
{
	static const float filter[9] =
	{
		0.02763055f,
		0.06628225f,
		0.12258900f,
		0.18017382f,
		0.20664876f,
		0.18017382f,
		0.12258900f,
		0.06628225f,
		0.02763055f,
	};

	// �t�B���^�[�Ɋ�Â��ĐF��擾
	float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);	// �o�͐F�̏�����
	float2 uvOffset = 1.0f / texSize;				// 1�s�N�Z����UV��ł̈ړ���
	uvOffset *= blurDir;							// �w�肳�ꂽ�����݈̂ړ�������
    float2 uv = pin.uv - uvOffset * 2;				// �����ʒu���炸�ꂽUV���W��v�Z
	
	// �t�B���^�[��K�p���ĐF��v�Z
	for (int i = 0; i < 9; ++i)
    {
		// �e�N�X�`������F��T���v�����O���ăt�B���^�[�������
		color += tex.Sample(samp, uv) * filter[i];
		uv += uvOffset;
	}

	return color;
}