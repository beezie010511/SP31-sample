#include "math.h"
#include "SceneMoveObject.h"
#include "Geometory.h"
#include "DebugLog.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void SceneMoveObject::Init()
{

}
void SceneMoveObject::Uninit()
{

}
void SceneMoveObject::Update(float tick)
{

}
void SceneMoveObject::Draw()
{
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixTranspose(
		DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f)
	));

	Geometory::SetWorld(mat);
	Geometory::DrawSphere();
}
