#include "SceneFur.h"
#include "Model.h"
#include "CameraBase.h"
#include "Input.h"

void SceneFur::Init()
{
	// シェーダーの読み込み処理
	Shader* shader[] = {
		CreateObj<VertexShader>("VS_Fur"),
		CreateObj<PixelShader>("PS_Fur"),
		CreateObj<GeometryShader>("GS_FurShell"),
		//CreateObj<GeometryShader>("GS_FurFin"),
		//CreateObj<GeometryShader>("GS_Fur"),
	};
	const char* file[] = {
		"Assets/Shader/VS_Fur.cso",
		"Assets/Shader/PS_Fur.cso",
		"Assets/Shader/GS_FurShell.cso",
		//"Assets/Shader/GS_FurFin.cso",
		//"Assets/Shader/GS_Fur.cso"
	};
	int shaderNum = _countof(shader);
	for (int i = 0; i < shaderNum; ++i)
	{
		if (FAILED(shader[i]->Load(file[i])))
		{
			MessageBox(NULL, file[i], "Shader Error", MB_OK);
		}
	}

	Texture* pTex;
	pTex = CreateObj<Texture>("ShellMask");
	pTex->Create("Assets/Texture/ShellMask.png");
	pTex = CreateObj<Texture>("FinMask");
	pTex->Create("Assets/Texture/FinMask.png");
}
void SceneFur::Uninit()
{
}
void SceneFur::Update(float tick)
{
}
void SceneFur::Draw()
{
	// 事前情報の取得
	Model* pModel = GetObj<Model>("Model");
	CameraBase* pCamera = GetObj<CameraBase>("Camera");
	Texture* pShellMask = GetObj<Texture>("ShellMask");
	Texture* pFinMask = GetObj<Texture>("FinMask");

	// 定数バッファに渡す行列の情報
	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());
	mat[1] = pCamera->GetView();
	mat[2] = pCamera->GetProj();
	// 定数バッファに渡すカメラの情報
	DirectX::XMFLOAT3 camPos = pCamera->GetPos();
	DirectX::XMFLOAT4 camera[] = {
		{camPos.x, camPos.y, camPos.z, 0.0f}
	};

	// シェーダーの取得
	Shader* shader[] = {
		GetObj<Shader>("VS_Fur"),
		GetObj<Shader>("PS_Fur"),
		GetObj<Shader>("GS_FurShell"),
		//GetObj<Shader>("GS_FurFin"),
		//GetObj<Shader>("GS_Fur"),
	};

	//--- シェル法の描画
	// 座標の設定
	DirectX::XMStoreFloat4x4(&mat[0],
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixTranslation(
				0.0f, 0.0f, 0.0f
			)));
	shader[1]->SetTexture(1, pShellMask);
	shader[2]->WriteBuffer(0, mat);

	// 描画
	pModel->SetVertexShader(shader[0]);
	pModel->SetPixelShader(shader[1]);

	shader[2]->Bind();
	pModel->Draw();
	GeometryShader::Unbind();

	/*
	//--- フィン法の描画
	// 座標の設定
	DirectX::XMStoreFloat4x4(&mat[0],
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixTranslation(
				1.0f, 0.0f, 0.0f
			)));
	shader[1]->SetTexture(1, pFinMask);
	shader[3]->WriteBuffer(0, mat);

	// 描画
	pModel->SetVertexShader(shader[0]);
	pModel->SetPixelShader(shader[1]);
	shader[3]->Bind();
	pModel->Draw();
	GeometryShader::Unbind();

	//--- シェル・フィン組み合わせた表示
	// 座標の設定
	DirectX::XMStoreFloat4x4(&mat[0],
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixTranslation(
				2.0f, 0.0f, 0.0f
			)));
	shader[1]->SetTexture(1, pShellMask);
	shader[1]->SetTexture(2, pFinMask);
	shader[1]->WriteBuffer(0, camera);
	shader[4]->WriteBuffer(0, mat);

	// 描画
	pModel->SetVertexShader(shader[0]);
	pModel->SetPixelShader(shader[1]);
	shader[4]->Bind();
	pModel->Draw();
	GeometryShader::Unbind();
	*/
}
