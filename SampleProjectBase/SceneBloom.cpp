#include "SceneBloom.h"
#include "Model.h"
#include "CameraBase.h""
#include "LightBase.h"
#include "Input.h"
#include "Sprite.h"
#include "Defines.h"

void SceneBloom::Init()
{
	// シェーダー読み込み
	Shader* shader[] = {
		CreateObj<VertexShader>("VS_Object"),
		CreateObj<PixelShader>("PS_TexColor"),
		CreateObj<PixelShader>("PS_Blur"),
		CreateObj<PixelShader>("PS_Luminance"),
		CreateObj<VertexShader>("VS_WorldPosition"),
		CreateObj<PixelShader>("PS_PhongSpecular"),
		CreateObj<PixelShader>("PS_PhongShading"),
	};
	const char* filePath[] = {
		"Assets/Shader/VS_Object.cso",
		"Assets/Shader/PS_TexColor.cso",
		"Assets/Shader/PS_Blur.cso",
		"Assets/Shader/PS_Luminance.cso",
		"Assets/Shader/VS_WorldPosition.cso",
		"Assets/Shader/PS_PhongSpecular.cso",
		"Assets/Shader/PS_PhongShading.cso",
	};
	static_assert(_countof(shader) == _countof(filePath), "");
	for (int i = 0; i < _countof(shader); ++i)
		shader[i]->Load(filePath[i]);

	// パラメータ設定
	m_index = 0;
	m_isMulti = false;

	// モデル読み込み
	Model* pField = GetObj<Model>("FieldModel");

	RenderTarget* pTexRTV = CreateObj<RenderTarget>("TexRTV");
	pTexRTV->Create(DXGI_FORMAT_R32G32B32A32_FLOAT, SCREEN_WIDTH, SCREEN_HEIGHT);

	RenderTarget* pLuminanceRTV = CreateObj<RenderTarget>("LuminanceRTV");
	pLuminanceRTV->Create(DXGI_FORMAT_R32G32B32A32_FLOAT, SCREEN_WIDTH, SCREEN_HEIGHT);

	// X方向ブラー用レンダーターゲット作成
	RenderTarget* pRTVxBlur = CreateObj<RenderTarget>("BlurX");
	pRTVxBlur->Create(DXGI_FORMAT_R32G32B32A32_FLOAT, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

	// Y方向ブラー用レンダーターゲット作成
	RenderTarget* pRTVyBlur = CreateObj<RenderTarget>("BlurY");
	pRTVyBlur->Create(DXGI_FORMAT_R32G32B32A32_FLOAT, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

	Sprite::SetVertexShader(nullptr);
	Sprite::SetPixelShader(nullptr);
}
void SceneBloom::Uninit()
{
}
void SceneBloom::Update(float tick)
{
	m_time += tick;
}
void SceneBloom::Draw()
{
	// 描画情報
	Model* pModel = GetObj<Model>("Model");
	Model* pField = GetObj<Model>("FieldModel");
	CameraBase* pCamera = GetObj<CameraBase>("Camera");
	LightBase* pLight = GetObj<LightBase>("Light");
	RenderTarget* pDefRTV = GetObj<RenderTarget>("RTV");
	DepthStencil* pDefDSV = GetObj<DepthStencil>("DSV");
	RenderTarget* pLuminanceRTV = GetObj<RenderTarget>("LuminanceRTV");
	RenderTarget* pTexRTV = GetObj<RenderTarget>("TexRTV");
	RenderTarget* pRTVxBlur = GetObj<RenderTarget>("BlurX");
	RenderTarget* pRTVyBlur = GetObj<RenderTarget>("BlurY");

	// 各種シェーダー取得
	Shader* shader[] = {
		GetObj<Shader>("VS_Object"),
		GetObj<Shader>("PS_TexColor"),
		nullptr, // Spriteのデフォルトシェーダー
		GetObj<Shader>("PS_Blur"),
		GetObj<Shader>("PS_Luminance"), // 4
		GetObj<Shader>("VS_WorldPosition"),
		GetObj<Shader>("PS_PhongSpecular"),
		GetObj<Shader>("PS_PhongShading"),
	};

	// 変換行列設定
	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());
	mat[0] = mat[0] * pModel->GetScaleBaseMatrix();
	mat[1] = pCamera->GetView();
	mat[2] = pCamera->GetProj();

	float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	pLuminanceRTV->Clear(color);
	pTexRTV->Clear(color);

	// 定数バッファに渡すライトの情報
	DirectX::XMFLOAT3 lightDir =
		pLight->GetDirection();
	DirectX::XMFLOAT4 light[] = {
		pLight->GetDiffuse(),
		pLight->GetAmbient(),
		{lightDir.x, lightDir.y, lightDir.z, 0.0f}
	};

	// 定数バッファに渡すカメラの情報
	DirectX::XMFLOAT3 camPos = pCamera->GetPos();
	DirectX::XMFLOAT4 camera[] = {
		{camPos.x, camPos.y, camPos.z, 0.0f}
	};

	// 描画先の変更
	SetRenderTargets(1, &pTexRTV, pDefDSV);

	SetBlendMode(BLEND_NONE);
	//SetCullingMode(D3D11_CULL_NONE);
	shader[5]->WriteBuffer(0, mat);
	shader[6]->WriteBuffer(0, light);
	shader[6]->WriteBuffer(1, camera);
	pModel->SetVertexShader(shader[5]);
	pModel->SetPixelShader(shader[6]);
	pModel->Draw();

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());
	mat[0] = mat[0] * pField->GetScaleBaseMatrix();
	shader[5]->WriteBuffer(0, mat);
	shader[7]->WriteBuffer(0, light);
	shader[7]->WriteBuffer(1, camera);
	pField->SetVertexShader(shader[5]);
	pField->SetPixelShader(shader[7]);
	pField->Draw();

	SetRenderTargets(1, &pLuminanceRTV, nullptr);

	// 輝度抽出
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(DirectX::XMMatrixOrthographicLH(1, 1, 0.0f, 1.0f)));
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetSize(DirectX::XMFLOAT2(1.0f, 1.0f));
	Sprite::SetPixelShader(shader[4]);
	Sprite::SetTexture(pTexRTV);
	Sprite::Draw();

	// ブラーでぼかす-------------------------------------------------------------------------------------------------
	SetRenderTargets(1, &pRTVxBlur, nullptr);
	// 横半分のレンダーターゲットへぼかしながら書き込み
	float blurParam[] = {
		SCREEN_WIDTH, SCREEN_HEIGHT, 4.0f, 0.0f
	};
	shader[3]->WriteBuffer(0, blurParam);
	Sprite::SetPixelShader(shader[3]);
	Sprite::SetTexture(pLuminanceRTV);
	Sprite::Draw();

	// Y方向ブラーテクスチャに描画
	SetRenderTargets(1, &pRTVyBlur, nullptr);

	// 縦半分のレンダーターゲットへぼかしながら書き込み
	blurParam[0] = SCREEN_WIDTH * 0.5f;
	blurParam[1] = SCREEN_HEIGHT * 0.5f;
	blurParam[2] = 0.0f;
	blurParam[3] = 4.0f;

	shader[3]->WriteBuffer(0, blurParam);
	Sprite::SetTexture(pRTVxBlur);
	Sprite::Draw();

	// 元の描画に戻す
	SetRenderTargets(1, &pDefRTV, m_isMulti ? pDefDSV : nullptr);
	Sprite::SetPixelShader(shader[2]);

	Sprite::SetTexture(pTexRTV);
	Sprite::Draw();

	SetBlendMode(BLEND_ADDALPHA);
	Sprite::SetTexture(pRTVyBlur);
	Sprite::Draw();

	// この後にグリッド線の描画を行うので、3Dに戻す
	SetBlendMode(BLEND_ALPHA);
	SetRenderTargets(1, &pDefRTV, pDefDSV);
}
