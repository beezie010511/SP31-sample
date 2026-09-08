#include <DirectXMath.h>

#include "Emitter.h"
#include "math.h"
#include "Geometory.h"
#include "DebugLog.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Emitter::Emitter(DirectX::XMFLOAT3 pos)
{
    m_pos = pos;
    XMStoreFloat4x4(&m_mat, XMMatrixTranspose(XMMatrixScaling(1.0f, 1.0f, 1.0f)));

    Init();
    mt.seed(rd());
}

void Emitter::Init()
{}

void Emitter::Update(float tick)
{
    m_time += tick;

    // ---------------------------------------------------------
    // LAUNCH MODE — spawn exactly ONE particle
    // ---------------------------------------------------------
    if (mode == Launch)
    {
        // If no particles exist, create ONE launch particle
        if (m_particles.empty())
        {
            CreateParticle();
        }
    }

    // ---------------------------------------------------------
    // Update all particles
    // ---------------------------------------------------------
    for (auto& p : m_particles)
    {
        p.Update(tick);
    }

    // ---------------------------------------------------------
    // Check if launch particle has stopped → trigger explosion
    // ---------------------------------------------------------
    if (mode == Launch)
    {
        for (auto& p : m_particles)
        {
            if (p.IsStop())
            {
                StartExplosion(p.GetPosition());
                mode = Explode;
                break;
            }
        }
    }
}

void Emitter::Drow()
{
    for (int i = 0; i < m_particles.size(); i++)
    {
        m_particles[i].Drow();
    }
}

void Emitter::CreateParticle()
{
    Particle p = Particle(
        m_pos,
        Rand_f(lifeTime_min, lifeTime_max),
        gravity,
        color_start,
        color_end,
        colorChangeStartRate,
        GenerateVelocity(),
        mass,
        refRate,
        Rand_f(deg_min, deg_max),
        texture
    );

    m_particles.push_back(p);
}

bool Emitter::RecycleParticle()
{
    // No recycling needed for fireworks
    return false;
}

DirectX::XMFLOAT3 Emitter::GenerateVelocity()
{
    float angle = Rand_f(0.0f, XM_2PI);
    float h = Rand_f(vecHorizontal_min, vecHorizontal_max);
    float v = Rand_f(vecVertical_min, vecVertical_max);

    return XMFLOAT3(h * cosf(angle), v, h * sinf(angle));
}

float Emitter::Rand_f(float min, float max)
{
    std::uniform_real_distribution<> urd(min, max);
    return urd(mt);
}

void Emitter::SetVector(float horizontalMin, float horizontalMax, float verticalMin, float verticalMax)
{
    vecHorizontal_min = horizontalMin;
    vecHorizontal_max = horizontalMax;
    vecVertical_min = verticalMin;
    vecVertical_max = verticalMax;
}

void Emitter::SetGeneratNum(float min, float max)
{
    m_generatNum_min = min;
    m_generatNum_max = max;
}

void Emitter::SetGeneratTime(float time)
{
    m_generatTime = time;
}

void Emitter::SetTexture(Texture* tex)
{
    texture = tex;
}

void Emitter::StartExplosion(XMFLOAT3 pos)
{
    int explosionCount = 50;

    for (int i = 0; i < explosionCount; i++)
    {
        Particle p = Particle(
            pos,
            Rand_f(0.8f, 1.2f),
            0.0f,
            XMFLOAT4(1, 0.5f, 0, 1),
            XMFLOAT4(1, 0, 0, 0),
            0.5f,
            GenerateExplosionVelocity(),
            1.0f,
            1.0f,
            0.0f,
            texture
        );

        m_particles.push_back(p);
    }
}

DirectX::XMFLOAT3 Emitter::GenerateExplosionVelocity()
{
    float angle1 = Rand_f(0.0f, XM_2PI);
    float angle2 = Rand_f(0.0f, XM_PI);
    float speed = Rand_f(5.0f, 15.0f);

    float x = speed * sin(angle2) * cos(angle1);
    float y = speed * cos(angle2);
    float z = speed * sin(angle2) * sin(angle1);

    return XMFLOAT3(x, y, z);
}

void Emitter::ClearParticles()
{
    m_particles.clear();
}
