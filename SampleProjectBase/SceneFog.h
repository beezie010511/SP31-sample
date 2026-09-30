#ifndef __SCENE_FOG_H__
#define __SCENE_FOG_H__

#include "SceneBase.hpp"

class SceneFog : public SceneBase
{
public:
	void Init();
	void Uninit();
	void Update(float tick);
	void Draw();
private:
	float m_time; // Œo‰ßŽžŠÔ

	enum FOG_TYPE {
		Distance = 1,
		Height = 2,
	};
	int FogIndex = FOG_TYPE::Distance;
};

#endif // __SCENE_FOG_H__