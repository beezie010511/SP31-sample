#ifndef __SCENE_WAVE_H__
#define __SCENE_WAVE_H__

#include "SceneBase.hpp"
#include "DirectXTex/SimpleMath.h"

#include <vector>

class SceneWave : public SceneBase
{
public:
	void Init();
	void Uninit();
	void Update(float tick);
	void Draw();

	struct LINE_SET {
		int p1_v;
		int p1_h;
		int p2_v;
		int p2_h;
		float length;
	};

private:
	const static int VERTEX_VERTICAL_MAX = 50;		// ���_�̐�_��
	const static int VERTEX_HORIZONTAL_MAX = 50;	// ���_�̐�_�c
	const static int SPRING_MAX =	(VERTEX_HORIZONTAL_MAX * (VERTEX_VERTICAL_MAX - 1)) +
									(VERTEX_VERTICAL_MAX * (VERTEX_HORIZONTAL_MAX - 1)) +
									((VERTEX_VERTICAL_MAX - 1) * (VERTEX_HORIZONTAL_MAX - 1) * 2);
	float m_time = 0.0f;

	const float PI = 3.141592f;
	const float DEFAULT_LENGTH = 1.0f;		// ���_�̊Ԋu
	const float WAVE_AMPLITUDE = 2.0f;		// �U��
	const float WAVE_LENGTH = 20.0f;		// �g�� ��
	const float WAVE_CYCLE = 2.0f;			// ���� T


	DirectX::XMFLOAT3 waveOrigin = DirectX::XMFLOAT3(DEFAULT_LENGTH * VERTEX_VERTICAL_MAX / 2 , 5.0f, DEFAULT_LENGTH * VERTEX_HORIZONTAL_MAX / 2);	// �g�̌��_
	DirectX::XMFLOAT3 wavePoint[VERTEX_VERTICAL_MAX][VERTEX_HORIZONTAL_MAX];	// �g���_�Ǘ��z��
	LINE_SET lineList[SPRING_MAX];										// ���C���Ǘ��z��
};

#endif // __SCENE_WAVE_H__