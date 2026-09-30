#include "SceneSpring.h"
#include "Geometory.h"
#include "Input.h"

void SceneSpring::Init()
{
	m_springPos = DirectX::XMFLOAT3(0.0f, 10.0f, 0.0f);
	m_ballPos = DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f);
}
void SceneSpring::Uninit()
{

}
void SceneSpring::Update(float tick)
{
	m_time += tick;
	bool isInput = false;

	if (IsKeyPress(VK_UP)){
		m_ballPos.z += 0.1f;
		isInput = true;
	}
	if (IsKeyPress(VK_LEFT)) {
		m_ballPos.x += -0.1f;
		isInput = true;
	}
	if (IsKeyPress(VK_DOWN)) {
		m_ballPos.z += -0.1f;
		isInput = true;
	}
	if (IsKeyPress(VK_RIGHT)) {
		m_ballPos.x += 0.1f;
		isInput = true;
	}
	if (IsKeyPress(VK_SPACE)) {
		m_ballPos.y += -0.1f;
		isInput = true;
	}

	// ばねの処理-------------------------------------------------------------------------------------
	// スプリングの長さを計算
	DirectX::XMFLOAT3 springLen = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	springLen.x = m_ballPos.x - m_springPos.x;
	springLen.y = m_ballPos.y - m_springPos.y;
	springLen.z = m_ballPos.z - m_springPos.z;
	float len = sqrtf(springLen.x * springLen.x + springLen.y * springLen.y + springLen.z * springLen.z);
	
	// スプリングのテンションを計算
	float tension = SPRING_COEFFICIENT * (len - DEFAULT_LENGTH);
	DirectX::XMFLOAT3 tensionVec = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	tensionVec.x = -1.0f * springLen.x * (tension / len);
	tensionVec.y = -1.0f * springLen.y * (tension / len);
	tensionVec.z = -1.0f * springLen.z * (tension / len);
	
	// 抵抗力を求める
	DirectX::XMFLOAT3 resistance = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	resistance.x = -m_velocity.x * ATT_COEFFICIENT;
	resistance.y = -m_velocity.y * ATT_COEFFICIENT;
	resistance.z = -m_velocity.z * ATT_COEFFICIENT;

	// かかる力を合成（合力＝重力＋張力＋抵抗力）
	DirectX::XMFLOAT3 resultant = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	resultant.x = m_gravity_vec.x + tensionVec.x + resistance.x;
	resultant.y = m_gravity_vec.y + tensionVec.y + resistance.y;
	resultant.z = m_gravity_vec.z + tensionVec.z + resistance.z;

	// 入力がある場合は以下の処理を止めて球の移動処理に逃がす
	if (isInput) {
		m_velocity.x = 0.0f;
		m_velocity.y = 0.0f;
		m_velocity.z = 0.0f;
		return;
	}


	// 合力と質量から加速度を求める
	m_acceleration.x = resultant.x / BALL_MASS;
	m_acceleration.y = resultant.y / BALL_MASS;
	m_acceleration.z = resultant.z / BALL_MASS;

	// 速度に加速度を加える
	m_velocity.x += m_acceleration.x;
	m_velocity.y += m_acceleration.y;
	m_velocity.z += m_acceleration.z;

	// 速度から座標を移動
	m_ballPos.x += m_velocity.x;
	m_ballPos.y += m_velocity.y;
	m_ballPos.z += m_velocity.z;
	
	// ばねの処理-------------------------------------------------------------------------------------
}
void SceneSpring::Draw()
{
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	Geometory::AddLine(m_springPos, color, m_ballPos, color);
	Geometory::DrawLines();

	// 球の座標行列作成
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixTranspose(
		DirectX::XMMatrixTranslation(m_ballPos.x, m_ballPos.y, m_ballPos.z)	// 球の座標
	));

	Geometory::SetWorld(mat);		// 行列セット
	Geometory::DrawSphere();		// 球描画
}
