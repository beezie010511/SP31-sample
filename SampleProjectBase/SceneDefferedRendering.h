#ifndef __SCENE_DEFFERED_RENDERING_H__
#define __SCENE_DEFFERED_RENDERING_H__

#include "SceneBase.hpp"
#include "CameraBase.h"

class SceneDefferedRendering : public SceneBase
{
public:
	void Init();
	void Uninit();
	void Update(float tick);
	void Draw();

private:
	bool m_isMulti;
	int m_index;
};

#endif // __SCENE_DEFFERED_RENDERING_H__