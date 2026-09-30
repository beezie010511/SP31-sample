#ifndef __SCENE_RUBBER_H__
#define __SCENE_RUBBER_H__

#include "SceneBase.hpp"
#include "DirectXTex/SimpleMath.h"
#include "Ball.h"

#include <vector>

class SceneRubber : public SceneBase
{
public:
	void Init();
	void Uninit();
	void Update(float tick);
	void Draw();

	struct SPRING {
		bool	onLock;		// 固定フラグ

		DirectX::XMFLOAT3	position;		// 位置
		DirectX::XMFLOAT3	velocity;		// 速度
		DirectX::XMFLOAT3	acceleration;	// 加速度
		DirectX::XMFLOAT3	resultant;		// 合力
		DirectX::XMFLOAT3	tension_up;		// 張力 上方向
		DirectX::XMFLOAT3	tension_down;	// 張力 下方向
	};

private:
	const static int SPRING_MAX = 10;			// バネ(球の数)
	float m_time = 0.0f;
	float m_speed = 1.0f;
	int selectIndex = 0;						// 操作できる球の番号

	const float DEFAULT_LENGTH = 1.0f;			// ばねの自然長
	const float SPRING_COEFFICIENT = 30.0f;		// ばね係数
	const float ATT_COEFFICIENT = 1.0f;			// 減衰係数(Attenuation)
	const float BALL_MASS = 2.0f;				// 質量

	DirectX::XMFLOAT3	gravity = DirectX::XMFLOAT3(0.0f, BALL_MASS * -0.98f, 0.0f);	// 重力
	SPRING springList[SPRING_MAX];	// パーティクル管理配列
};

#endif // __SCENE_RUBBER_H__