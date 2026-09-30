#include "SceneRubber.h"
#include "Geometory.h"
#include "Input.h"

#include "DebugLog.h"

void SceneRubber::Init()
{
	for (int i = 0; i < SPRING_MAX; i++)
	{
		SPRING spring;

		if (i == 0) {
			spring.onLock = true;
		}
		else {
			spring.onLock = false;
		}
		spring.position = DirectX::XMFLOAT3(5.0f, 10.0f - i * DEFAULT_LENGTH, 5.0f);
		spring.velocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		springList[i] = spring;
	}
}
void SceneRubber::Uninit()
{

}
void SceneRubber::Update(float tick)
{
	m_time += tick;
	bool isInput = false;

	// 球(バネ)の移動
	if (IsKeyPress(VK_UP)){
		springList[selectIndex].position.z += 0.3f;
		isInput = true;
	}
	if (IsKeyPress(VK_LEFT)) {
		springList[selectIndex].position.x += -0.3f;
		isInput = true;
	}
	if (IsKeyPress(VK_DOWN)) {
		springList[selectIndex].position.z += -0.3f;
		isInput = true;
	}
	if (IsKeyPress(VK_RIGHT)) {
		springList[selectIndex].position.x += 0.3f;
		isInput = true;
	}
	if (IsKeyPress(VK_SPACE)) {
		springList[selectIndex].position.y += -0.3f;
		isInput = true;
	}
	if (IsKeyPress(VK_CONTROL)) {
		springList[selectIndex].position.y += 0.3f;
		isInput = true;
	}

	// 操作する球(バネ)を変更
	if (IsKeyTrigger('Z')) {
		selectIndex++;
		if (selectIndex >= SPRING_MAX) selectIndex = 0;
	}
	if (IsKeyTrigger('X')) {
		selectIndex--;
		if (selectIndex < 0) selectIndex = SPRING_MAX - 1;
	}

	// ばね更新処理-------------------------------------------------------------------------------------
	for (int i = 0; i < SPRING_MAX; i++)
	{
		const DirectX::XMFLOAT3 zero(0.0f, 0.0f, 0.0f);
		springList[i].tension_up = zero;
		springList[i].tension_down = zero;

		if (i > 0)
		{
			DirectX::XMFLOAT3 vec = DirectX::XMFLOAT3(
				springList[i - 1].position.x - springList[i].position.x,
				springList[i - 1].position.y - springList[i].position.y,
				springList[i - 1].position.z - springList[i].position.z);
			float length = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
			if (length > 0.0001f)
			{
				float tension = SPRING_COEFFICIENT * (length - DEFAULT_LENGTH);
				springList[i].tension_up = DirectX::XMFLOAT3(
					vec.x * tension / length,
					vec.y * tension / length,
					vec.z * tension / length);
			}
		}

		if (i + 1 < SPRING_MAX)
		{
			DirectX::XMFLOAT3 vec = DirectX::XMFLOAT3(
				springList[i + 1].position.x - springList[i].position.x,
				springList[i + 1].position.y - springList[i].position.y,
				springList[i + 1].position.z - springList[i].position.z);
			float length = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
			if (length > 0.0001f)
			{
				float tension = SPRING_COEFFICIENT * (length - DEFAULT_LENGTH);
				springList[i].tension_down = DirectX::XMFLOAT3(
					vec.x * tension / length,
					vec.y * tension / length,
					vec.z * tension / length);
			}
		}

		DirectX::XMFLOAT3 resistance(
			-springList[i].velocity.x * ATT_COEFFICIENT,
			-springList[i].velocity.y * ATT_COEFFICIENT,
			-springList[i].velocity.z * ATT_COEFFICIENT);
		springList[i].resultant = DirectX::XMFLOAT3(
			gravity.x + springList[i].tension_up.x + springList[i].tension_down.x + resistance.x,
			gravity.y + springList[i].tension_up.y + springList[i].tension_down.y + resistance.y,
			gravity.z + springList[i].tension_up.z + springList[i].tension_down.z + resistance.z);

		if (springList[i].onLock) continue;
		if (isInput && i == selectIndex)
		{
			springList[i].velocity = zero;
			continue;
		}

		springList[i].acceleration = DirectX::XMFLOAT3(
			springList[i].resultant.x / BALL_MASS,
			springList[i].resultant.y / BALL_MASS,
			springList[i].resultant.z / BALL_MASS);
		springList[i].velocity.x += springList[i].acceleration.x * tick;
		springList[i].velocity.y += springList[i].acceleration.y * tick;
		springList[i].velocity.z += springList[i].acceleration.z * tick;
		springList[i].position.x += springList[i].velocity.x * tick;
		springList[i].position.y += springList[i].velocity.y * tick;
		springList[i].position.z += springList[i].velocity.z * tick;
	}
}
void SceneRubber::Draw()
{
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	for (int i = 0; i < SPRING_MAX; i++)
	{
		if (i != 0) {
			Geometory::AddLine(springList[i].position, color, springList[i - 1].position, color);
			Geometory::DrawLines();
		}

		// 球の座標行列作成
		DirectX::XMFLOAT4X4 mat;
		DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixTranspose(
			DirectX::XMMatrixTranslation(springList[i].position.x, springList[i].position.y, springList[i].position.z)	// 球の座標
		));

		Geometory::SetWorld(mat);		// 行列セット

		// 選択中の球(バネ)は色を変える
		if(i == selectIndex) Geometory::SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		else Geometory::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		Geometory::DrawSphere();		// 球描画
	}
}
