//
//  TestScene.h
//  model3d
//
//  Created by 魏裕群 on 14-4-21.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__TestScene__
#define __model3d__TestScene__

#include "Scene.h"
#include "Mesh.h"
class TestScene:public Scene{
private:
	Mesh *m_pmesh;
	float deg;
	float start_x;
public:
	inline TestScene():deg(0){}
	void initRes();
	virtual bool onTouchEvent(MotionEvent &event);
};

#endif /* defined(__model3d__TestScene__) */
