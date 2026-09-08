#ifndef __EMITTER_H__
#define __EMITTER_H__

#include <vector>
#include <random>
#include "Texture.h"
#include "Particle.h"
#include "DirectXTex/SimpleMath.h"

#pragma once

class Emitter
{
public:
    enum FireworkMode {
        Launch,
        Explode
    };

public:
    Emitter(DirectX::XMFLOAT3 pos);
    void Init();
    void Update(float tick);
    void Drow();
    void CreateParticle();
    bool RecycleParticle();
    void SetVector(float horizontalMin, float horizontalMax, float verticalMin, float verticalMax);
    void SetGeneratNum(float min, float max);
    void SetGeneratTime(float time);
    void SetTexture(Texture* tex);
    void ClearParticles();

    // Firework functions
    void StartExplosion(DirectX::XMFLOAT3 pos);
    DirectX::XMFLOAT3 GenerateExplosionVelocity();

public:
    DirectX::XMFLOAT3 m_pos;

    float lifeTime_min = 1.0f;
    float lifeTime_max = 1.0f;
    float gravity = 9.8f;

    DirectX::XMFLOAT4 color_start = DirectX::XMFLOAT4(1, 1, 1, 1);
    DirectX::XMFLOAT4 color_end = DirectX::XMFLOAT4(0, 0, 0, 1);
    float colorChangeStartRate = 0.8f;

    float vecHorizontal_min = 0.0f;
    float vecHorizontal_max = 0.0f;
    float vecVertical_min = 0.0f;
    float vecVertical_max = 0.0f;

    float deg_min = 0.0f;
    float deg_max = 0.0f;

    float mass = 1.0f;
    float refRate = 1.0f;

    Texture* texture = nullptr;

    FireworkMode mode = Launch;

private:
    const int m_particleMax = 1000;
    DirectX::XMFLOAT4X4 m_mat;

    float m_time = 0.0f;
    std::vector<Particle> m_particles;

    int m_generatNum_min = 1;
    int m_generatNum_max = 1;
    float m_generatTime = 0.1f;
    float m_generatTimeCount = 0.0f;

    std::random_device rd;
    std::mt19937 mt;

    float Rand_f(float min, float max);
    DirectX::XMFLOAT3 GenerateVelocity();
};

#endif
