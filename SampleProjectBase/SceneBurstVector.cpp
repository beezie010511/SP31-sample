//#include <DirectXMath.h>
#include "math.h"
#include "SceneBurstVector.h"
#include "Geometory.h"
#include "DebugLog.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void SceneBurstVector::Init()
{
	Ball b1 = Ball(XMFLOAT3(-10.0f, 0.0f, 0.0f));
	b1.m_speedVec = (XMFLOAT3(5.0f, 0.0f, 0.0f));
	b1.SetColor(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	m_Balls.push_back(b1);

	Ball b2 = Ball(XMFLOAT3(10.0f, 0.0f, 0.0f));
	b2.m_speedVec = (XMFLOAT3(-6.0f, 0.0f, 0.0f));
	b2.SetColor(XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	m_Balls.push_back(b2);

	Ball b3 = Ball(XMFLOAT3(-5.0f, 0.0f, 10.0f));
	b3.m_speedVec = (XMFLOAT3(3.0f, 0.0f, -5.0f));
	b3.SetColor(XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
	m_Balls.push_back(b3);
}
void SceneBurstVector::Uninit()
{

}
void SceneBurstVector::Update(float tick)
{
    m_time += tick;

    // 1. Move balls
    for (auto& ball : m_Balls)
    {
        Vector3 pos = Vector3(ball.m_pos.x, ball.m_pos.y, ball.m_pos.z);
        Vector3 vel = Vector3(ball.m_speedVec.x, ball.m_speedVec.y, ball.m_speedVec.z);

        pos += vel * tick;

        ball.m_pos = XMFLOAT3(pos.x, pos.y, pos.z);
    }

    // 2. Collision detection + response
    for (int i = 0; i < m_Balls.size(); i++)
    {
        for (int j = i + 1; j < m_Balls.size(); j++)
        {
            if (m_Balls[i].HitSphere(m_Balls[j]))
            {
                // Convert to Vector3
                Vector3 posA = Vector3(m_Balls[i].GetPosition().x, m_Balls[i].GetPosition().y, m_Balls[i].GetPosition().z);
                Vector3 posB = Vector3(m_Balls[j].GetPosition().x, m_Balls[j].GetPosition().y, m_Balls[j].GetPosition().z);

                Vector3 velA = Vector3(m_Balls[i].m_speedVec.x, m_Balls[i].m_speedVec.y, m_Balls[i].m_speedVec.z);
                Vector3 velB = Vector3(m_Balls[j].m_speedVec.x, m_Balls[j].m_speedVec.y, m_Balls[j].m_speedVec.z);

                // Collision normal
                Vector3 normal = posA - posB;
                normal.Normalize();

                // Reflect velocities
                velA = velA - 2 * velA.Dot(normal) * normal;
                velB = velB - 2 * velB.Dot(normal) * normal;

                // Write back
                m_Balls[i].m_speedVec = XMFLOAT3(velA.x, velA.y, velA.z);
                m_Balls[j].m_speedVec = XMFLOAT3(velB.x, velB.y, velB.z);

                // Slight separation to avoid sticking
                m_Balls[i].m_pos.x += normal.x * 0.1f;
                m_Balls[i].m_pos.y += normal.y * 0.1f;
                m_Balls[i].m_pos.z += normal.z * 0.1f;

                m_Balls[j].m_pos.x -= normal.x * 0.1f;
                m_Balls[j].m_pos.y -= normal.y * 0.1f;
                m_Balls[j].m_pos.z -= normal.z * 0.1f;
            }
        }
    }
}

void SceneBurstVector::Draw()
{
	for (int i = 0; i < m_Balls.size(); i++)
	{
		m_Balls[i].Draw();
	}
}
