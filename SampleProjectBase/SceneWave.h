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
	const static int VERTEX_VERTICAL_MAX = 50;		// í∏ì_ÇÃêî_â°
	const static int VERTEX_HORIZONTAL_MAX = 50;	// í∏ì_ÇÃêî_èc
	const static int SPRING_MAX =	(VERTEX_HORIZONTAL_MAX * (VERTEX_VERTICAL_MAX - 1)) +
									(VERTEX_VERTICAL_MAX * (VERTEX_HORIZONTAL_MAX - 1)) +
									((VERTEX_VERTICAL_MAX - 1) * (VERTEX_HORIZONTAL_MAX - 1) * 2);
	float m_time = 0.0f;

	const float PI = 3.141592f;
	const float DEFAULT_LENGTH = 1.0f;		// í∏ì_ÇÃä‘äu
	const float WAVE_AMPLITUDE = 2.0f;		// êUïù
	const float WAVE_LENGTH = 20.0f;		// îgí∑ É…
	const float WAVE_CYCLE = 2.0f;			// é¸ä˙ T


	DirectX::XMFLOAT3 waveOrigin = DirectX::XMFLOAT3(DEFAULT_LENGTH * VERTEX_VERTICAL_MAX / 2 , 5.0f, DEFAULT_LENGTH * VERTEX_HORIZONTAL_MAX / 2);	// îgÇÃå¥ì_
	DirectX::XMFLOAT3 wavePoint[VERTEX_VERTICAL_MAX][VERTEX_HORIZONTAL_MAX];	// îgí∏ì_ä«óùîzóÒ
	LINE_SET lineList[SPRING_MAX];										// ÉâÉCÉìä«óùîzóÒ
};

#endif // __SCENE_WAVE_H__