#ifndef __SCENE_BLOOM_H__
#define __SCENE_BLOOM_H__

#include "SceneBase.hpp"

class SceneBloom : public SceneBase
{
public:
	void Init();
	void Uninit();
	void Update(float tick);
	void Draw();
private:
	float m_time;
	int m_index = 0;
	bool m_isMulti = false;
};

#endif // __SCENE_BLOOM_H__
