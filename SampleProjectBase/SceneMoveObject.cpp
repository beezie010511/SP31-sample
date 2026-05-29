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
    static int mode = 0;     // 0–5
    static float time = 0.0f;

    // Switch mode when F is pressed
    if (GetAsyncKeyState('F') & 0x0001)
    {
        mode = (mode + 1) % 6;
        time = 0.0f;         // reset time for new motion
    }

    time += 0.016f; // or tick if you want
    float x = 0, y = 0, z = 0;
    float v0 = 5.0f;
    float a = 15.0f;
    float g = -9.8f;
    float t = time;

    switch (mode)
    {
    case 0: // ① 等速運動
        x = v0 * t;
        break;

    case 1: // ② 等加速度運動
        x = v0 * t + 0.5f * a * t * t;
        break;

    case 2: // ③ 自由落下運動
        y = 0.5f * g * t * t;
        break;

    case 3: // ④ 鉛直投げ上げ
        y = v0 * t + 0.5f * g * t * t;
        break;

    case 4: // ⑤ 水平投射
        x = v0 * t;
        y = 0.5f * g * t * t;
        break;

    case 5: // ⑥ 斜方投射
    {
        float angle = XMConvertToRadians(45.0f);
        x = v0 * cosf(angle) * t;
        y = v0 * sinf(angle) * t + 0.5f * g * t * t;
    }
    break;
    }
    DirectX::XMFLOAT4X4 mat;
    XMStoreFloat4x4(&mat, XMMatrixTranspose(
        XMMatrixScaling(0.3f, 0.3f, 0.3f) *
        XMMatrixTranslation(x, y, z)
    ));

    Geometory::SetWorld(mat);
    Geometory::DrawSphere();
}
