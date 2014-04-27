//
//  TestScene.cpp
//  model3d
//
//  Created by 魏裕群 on 14-4-21.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "TestScene.h"

void TestScene::initRes(){
	m_pmesh=new Mesh();
	//下面两个方法要在load后加载
	m_pmesh->setForceGenerateNormal(true);
	m_pmesh->setSmoothSurface(false);
#ifdef __APPLE__
	m_pmesh->load("/Users/anan/Documents/github/model3d/assert/peri/peri.obj");
#else
	m_pmesh->load("../assert/peri/peri.obj");
#endif
	m_pmesh->setUseShader(true);
	GLfloat hy = m_pmesh->getMaxY();
	GLfloat ly = m_pmesh->getMinY();
	m_pmesh->setRotationAxis(0, 1, 0);

	//let model show us
	GLfloat scale = 18/(hy-ly);
	m_pmesh->setScale(scale,scale,scale);
	m_pmesh->setPosition(0, -9.0f*(hy+ly)/(hy-ly), -20);

	addSubNode(m_pmesh);
}

bool TestScene::onTouchEvent(MotionEvent &event){
	//printf("%d(%f,%f)\n",event.action,event.x,event.y);
	switch (event.action)
	{
	case MotionDown:
		start_x=event.x;
		break;
	case MotionMove:
		m_pmesh->setDegree(deg+10*(event.x-start_x));
		break;
	case MotionCancel:
	case MotionUp:
		deg=deg+10*(event.x-start_x);
	default:
		break;
	}
	return false;
}
