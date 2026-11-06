#include "SceneWave.h"
#include "Geometory.h"
#include "Input.h"

#include "DebugLog.h"

void SceneWave::Init()
{
	// 頂点の初期化
	for (int i = 0; i < VERTEX_VERTICAL_MAX; i++)
	{
		for (int j = 0; j < VERTEX_HORIZONTAL_MAX; j++)
		{
			wavePoint[i][j] = DirectX::XMFLOAT3(DEFAULT_LENGTH * i, 5.0f, DEFAULT_LENGTH * j);
		}
	}

	//	ラインの初期化
	int		count = 0;
	for (int i = 0; i < VERTEX_VERTICAL_MAX; i++)
	{
		for (int j = 0; j < VERTEX_HORIZONTAL_MAX; j++)
		{
			if (i < VERTEX_VERTICAL_MAX - 1) {
				lineList[count].p1_v = i;		// １個目の参照粒子インデックスを設定
				lineList[count].p1_h = j;
				lineList[count].p2_v = i + 1;	// ２個目の参照粒子インデックスを設定
				lineList[count].p2_h = j;
				count++;
			}
			if (j < VERTEX_HORIZONTAL_MAX - 1) {
				lineList[count].p1_v = i;		// １個目の参照粒子インデックスを設定
				lineList[count].p1_h = j;
				lineList[count].p2_v = i;		// ２個目の参照粒子インデックスを設定
				lineList[count].p2_h = j + 1;
				count++;
			}
			if (i < VERTEX_VERTICAL_MAX - 1 && j < VERTEX_HORIZONTAL_MAX - 1) {
				lineList[count].p1_v = i;			// １個目の参照粒子インデックスを設定
				lineList[count].p1_h = j;
				lineList[count].p2_v = i + 1;		// ２個目の参照粒子インデックスを設定
				lineList[count].p2_h = j + 1;
				count++;
			}
			if (i < VERTEX_VERTICAL_MAX - 1 && j > 0) {
				lineList[count].p1_v = i;			// １個目の参照粒子インデックスを設定
				lineList[count].p1_h = j;
				lineList[count].p2_v = i + 1;		// ２個目の参照粒子インデックスを設定
				lineList[count].p2_h = j - 1;
				count++;
			}
		}
	}
}
void SceneWave::Uninit()
{

}
void SceneWave::Update(float tick)
{
	m_time += tick;

	float moveSpeed = 10.0f * tick;

	if (IsKeyPress(VK_UP))
	{
		waveOrigin.z -= moveSpeed;
	}

	if (IsKeyPress(VK_DOWN))
	{
		waveOrigin.z += moveSpeed;
	}

	if (IsKeyPress(VK_LEFT))
	{
		waveOrigin.x -= moveSpeed;
	}

	if (IsKeyPress(VK_RIGHT))
	{
		waveOrigin.x += moveSpeed;
	}


	for (int i = 0; i < VERTEX_VERTICAL_MAX; i++)
	{
		for (int j = 0; j < VERTEX_HORIZONTAL_MAX; j++)
		{
			float x = DEFAULT_LENGTH * i;
			float z = DEFAULT_LENGTH * j;

			float dx = x - waveOrigin.x;
			float dz = z - waveOrigin.z;

			float dist = sqrtf(dx * dx + dz * dz);

			float amplitude = 3.0f;      // wave height
			float frequency = 0.3f;       // wave spacing
			float speed = 3.0f;           // propagation speed

			float y = amplitude * sinf(dist * frequency - m_time * speed);

			wavePoint[i][j].x = x;
			wavePoint[i][j].y = y;
			wavePoint[i][j].z = z;
		}
	}
}

void SceneWave::Draw()
{
	DirectX::XMFLOAT4 color_s = DirectX::XMFLOAT4(0.1f, 0.1f, 0.6f, 1.0f);
	DirectX::XMFLOAT4 color_e = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	for (int i = 0; i < SPRING_MAX; i++)
	{
		int v1 = lineList[i].p1_v;
		int h1 = lineList[i].p1_h;
		int v2 = lineList[i].p2_v;
		int h2 = lineList[i].p2_h;
		Geometory::AddLine(wavePoint[v1][h1], color_s, wavePoint[v2][h2], color_e);
	}
}
