#ifndef __SCENE_MOVE_OBJECT__
#define __SCENE_MOVE_OBJECT__

#include "SceneBase.hpp"
#include "Ball.h"
#include <vector>

class SceneMoveObject : public SceneBase
{
public:
	void Init();
	void Uninit();
	void Update(float tick);
	void Draw();
private:

};

#endif // __SCENE_MOVE_OBJECT__