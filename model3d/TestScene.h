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
	Light *m_plight;
	Light *m_plight1;
	float deg;
	float start_x;
	int l1;
public:
	inline TestScene():deg(0),l1(0){}
	void operator()();
	void initRes();
    void complete(Drawable* node);
    void update(Drawable* node,float r);
	virtual bool onTouchEvent(MotionEvent &event);
	virtual bool onKeyEvent(KeyEvent &event);
};

#endif /* defined(__model3d__TestScene__) */
