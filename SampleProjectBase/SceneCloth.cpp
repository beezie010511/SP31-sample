#include "SceneCloth.h"
#include "Geometory.h"
#include "Input.h"

#include "DebugLog.h"

void SceneCloth::Init()
{
	// 粒子(頂点)の初期化
	for (int i = 0; i < VERTEX_VERTICAL_MAX; i++)
	{
		for (int j = 0; j < VERTEX_HORIZONTAL_MAX; j++)
		{
			SPRING_VECTOR& vertex = vertexList[i][j];
			vertex.onLock = (j == 0);
			vertex.position = DirectX::XMFLOAT3(
				j * HORIZONTAL_SPACING,
				10.0f - i * DEFAULT_LENGTH,
				0.0f);
			vertex.velocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			vertex.acceleration = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			vertex.resultant = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
	}

	//	バネの初期化
	int		count = 0;			// バネカウント用
	auto addSpring = [&](int v1, int h1, int v2, int h2)
	{
		lineList[count].p1_v = v1;
		lineList[count].p1_h = h1;
		lineList[count].p2_v = v2;
		lineList[count].p2_h = h2;
		DirectX::XMFLOAT3 diff(
			vertexList[v2][h2].position.x - vertexList[v1][h1].position.x,
			vertexList[v2][h2].position.y - vertexList[v1][h1].position.y,
			vertexList[v2][h2].position.z - vertexList[v1][h1].position.z);
		lineList[count].length = sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
		++count;
	};

	for (int i = 0; i < VERTEX_VERTICAL_MAX; i++)
	{
		for (int j = 0; j < VERTEX_HORIZONTAL_MAX; j++)
		{
			if (j + 1 < VERTEX_HORIZONTAL_MAX)
				addSpring(i, j, i, j + 1);
			if (i + 1 < VERTEX_VERTICAL_MAX)
				addSpring(i, j, i + 1, j);
			if (i + 1 < VERTEX_VERTICAL_MAX && j + 1 < VERTEX_HORIZONTAL_MAX)
				addSpring(i, j, i + 1, j + 1);
			if (i + 1 < VERTEX_VERTICAL_MAX && j > 0)
				addSpring(i, j, i + 1, j - 1);
		}
	}

}
void SceneCloth::Uninit()
{

}
void SceneCloth::Update(float tick)
{
	m_time += tick;

	if (IsKeyTrigger('Z'))
		onWind = !onWind;
	const float windTurnSpeed = 2.0f;
	if (IsKeyPress(VK_UP))
		windDirection.z += windTurnSpeed * tick;
	if (IsKeyPress(VK_DOWN))
		windDirection.z -= windTurnSpeed * tick;
	if (IsKeyPress(VK_LEFT))
		windDirection.x -= windTurnSpeed * tick;
	if (IsKeyPress(VK_RIGHT))
		windDirection.x += windTurnSpeed * tick;

	float directionLength = sqrtf(
		windDirection.x * windDirection.x +
		windDirection.y * windDirection.y +
		windDirection.z * windDirection.z);
	if (directionLength > 0.0001f)
	{
		windforce = DirectX::XMFLOAT3(
			windDirection.x / directionLength * windPower,
			windDirection.y / directionLength * windPower,
			windDirection.z / directionLength * windPower);
	}
	else
	{
		windforce = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	for (int v = 0; v < VERTEX_VERTICAL_MAX; ++v)
	{
		for (int h = 0; h < VERTEX_HORIZONTAL_MAX; ++h)
		{
			SPRING_VECTOR& particle = vertexList[v][h];
			particle.resultant = gravity;
			if (!particle.onLock)
			{
				particle.resultant.x -= particle.velocity.x * ATT_COEFFICIENT;
				particle.resultant.y -= particle.velocity.y * ATT_COEFFICIENT;
				particle.resultant.z -= particle.velocity.z * ATT_COEFFICIENT;
				if (onWind)
				{
					particle.resultant.x += windforce.x;
					particle.resultant.y += windforce.y;
					particle.resultant.z += windforce.z;
				}
			}

	// Position constraints keep the cloth nearly inextensible under wind.
	for (int iteration = 0; iteration < 1; ++iteration)
	{
		for (int i = 0; i < SPRING_MAX; ++i)
		{
			SPRING_LINE& spring = lineList[i];
			SPRING_VECTOR& p1 = vertexList[spring.p1_v][spring.p1_h];
			SPRING_VECTOR& p2 = vertexList[spring.p2_v][spring.p2_h];
			DirectX::XMFLOAT3 delta(
				p2.position.x - p1.position.x,
				p2.position.y - p1.position.y,
				p2.position.z - p1.position.z);
			float length = sqrtf(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
			if (length <= 0.0001f)
				continue;

			float correction = (length - spring.length) / length;
			correction *= 0.9f;
			if (p1.onLock)
			{
				p2.position.x -= delta.x * correction;
				p2.position.y -= delta.y * correction;
				p2.position.z -= delta.z * correction;
			}
			else if (p2.onLock)
			{
				p1.position.x += delta.x * correction;
				p1.position.y += delta.y * correction;
				p1.position.z += delta.z * correction;
			}
			else
			{
				p1.position.x += delta.x * correction * 0.5f;
				p1.position.y += delta.y * correction * 0.5f;
				p1.position.z += delta.z * correction * 0.5f;
				p2.position.x -= delta.x * correction * 0.5f;
				p2.position.y -= delta.y * correction * 0.5f;
				p2.position.z -= delta.z * correction * 0.5f;
			}
		}
	}
		}
	}

	for (int i = 0; i < SPRING_MAX; ++i)
	{
		SPRING_LINE& spring = lineList[i];
		SPRING_VECTOR& p1 = vertexList[spring.p1_v][spring.p1_h];
		SPRING_VECTOR& p2 = vertexList[spring.p2_v][spring.p2_h];
		DirectX::XMFLOAT3 delta(
			p2.position.x - p1.position.x,
			p2.position.y - p1.position.y,
			p2.position.z - p1.position.z);
		float length = sqrtf(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
		if (length <= 0.0001f)
			continue;

		float force = SPRING_COEFFICIENT * (length - spring.length);
		float fx = delta.x / length * force;
		float fy = delta.y / length * force;
		float fz = delta.z / length * force;
		p1.resultant.x += fx;
		p1.resultant.y += fy;
		p1.resultant.z += fz;
		p2.resultant.x -= fx;
		p2.resultant.y -= fy;
		p2.resultant.z -= fz;
	}

	for (int v = 0; v < VERTEX_VERTICAL_MAX; ++v)
	{
		for (int h = 0; h < VERTEX_HORIZONTAL_MAX; ++h)
		{
			SPRING_VECTOR& particle = vertexList[v][h];
			if (particle.onLock)
				continue;

			particle.acceleration = DirectX::XMFLOAT3(
				particle.resultant.x / BALL_MASS,
				particle.resultant.y / BALL_MASS,
				particle.resultant.z / BALL_MASS);
			particle.velocity.x += particle.acceleration.x * tick;
			particle.velocity.y += particle.acceleration.y * tick;
			particle.velocity.z += particle.acceleration.z * tick;
			particle.position.x += particle.velocity.x * tick;
			particle.position.y += particle.velocity.y * tick;
			particle.position.z += particle.velocity.z * tick;
		}
	}
}
void SceneCloth::Draw()
{
	DirectX::XMFLOAT4 color_s = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT4 color_e = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	for (int i = 0; i < SPRING_MAX; i++)
	{
		int v1 = lineList[i].p1_v;
		int h1 = lineList[i].p1_h;
		int v2 = lineList[i].p2_v;
		int h2 = lineList[i].p2_h;
		Geometory::AddLine(vertexList[v1][h1].position, color_s, vertexList[v2][h2].position, color_e);
	}
	Geometory::DrawLines();

	Geometory::AddLine(
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f),
		DirectX::XMFLOAT3(0.0f, 10.0f, 0.0f),
		DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f));
	Geometory::DrawLines();
}
