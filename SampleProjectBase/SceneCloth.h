#ifndef __SCENE_CLOTH_H__
#define __SCENE_CLOTH_H__

#include "SceneBase.hpp"
#include "DirectXTex/SimpleMath.h"
#include "Ball.h"

#include <vector>

class SceneCloth : public SceneBase
{
public:
	void Init();
	void Uninit();
	void Update(float tick);
	void Draw();

	struct SPRING_VECTOR {
		bool	onLock;		// �Œ�t���O

		//  ����t���O
		DirectX::XMFLOAT3	position;		// �ʒu
		DirectX::XMFLOAT3	velocity;		// ���x
		DirectX::XMFLOAT3	acceleration;	// �����x
		DirectX::XMFLOAT3	resultant;		// ����
	};

	struct SPRING_LINE {
		int p1_v;
		int p1_h;
		int p2_v;
		int p2_h;
		float length;
	};

private:
	const static int VERTEX_VERTICAL_MAX = 20;		// ���_�̐�_��
	const static int VERTEX_HORIZONTAL_MAX = 20;	// ���_�̐�_�c
	const static int SPRING_MAX = (VERTEX_HORIZONTAL_MAX * (VERTEX_VERTICAL_MAX - 1)) +
								  (VERTEX_VERTICAL_MAX * (VERTEX_HORIZONTAL_MAX - 1)) +
								  ((VERTEX_VERTICAL_MAX -1) * (VERTEX_HORIZONTAL_MAX -1) * 2);	// �X�v�����O����
	float m_time = 0.0f;
	float m_speed = 1.0f;
	int selectIndex = 0;						// ����ł��鋅�̔ԍ�
	bool onWind = true;
	float windPower = 20.0f;
	DirectX::XMFLOAT3 windDirection = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);

	const float DEFAULT_LENGTH = 0.35f;		// �΂˂̎��R��
	const float HORIZONTAL_SPACING = 0.4f;
	float SPRING_COEFFICIENT = 250.0f;		// �΂ˌW��
	const float ATT_COEFFICIENT = 3.0f;		// �����W��(Attenuation)
	const float BALL_MASS = 0.3f;			// ����

	DirectX::XMFLOAT3 gravity = DirectX::XMFLOAT3(0.0f, BALL_MASS * -19.6f, 0.0f);
	DirectX::XMFLOAT3 windforce = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);				// ����
	SPRING_VECTOR vertexList[VERTEX_VERTICAL_MAX][VERTEX_HORIZONTAL_MAX];	// �z���_�Ǘ��z��
	SPRING_LINE lineList[SPRING_MAX];										// �S���Ǘ��z��
};

#endif // __SCENE_CLOTH_H__