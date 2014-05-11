//
//  TestScene.cpp
//  model3d
//
//  Created by 魏裕群 on 14-4-21.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "TestScene.h"

void TestScene::initRes(){
	m_plight = new Light();
	m_plight->position[0]=1;
	m_plight->position[1]=0;
	m_plight->position[2]=0;
	m_plight->position[3]=0;

	m_plight->ambient[0]=.35f;
	m_plight->ambient[1]=.35f;
	m_plight->ambient[2]=.35f;
	m_plight->ambient[3]=1;

	m_plight->diffuse[0]=1;
	m_plight->diffuse[1]=1;
	m_plight->diffuse[2]=1;
	m_plight->diffuse[3]=1;

	m_plight->specular[0]=1;
	m_plight->specular[1]=1;
	m_plight->specular[2]=1;
	m_plight->specular[3]=0;

	m_plight->enabled=true;

	addLight(m_plight);

	m_pmesh=new Mesh();
	//下面两个方法要在load后加载
	m_pmesh->setForceGenerateNormal(true);
	m_pmesh->setSmoothSurface(false);
#ifdef __APPLE__
	m_pmesh->load("/Users/anan/Documents/github/model3d/assets/peri/peri.obj");
#else
	m_pmesh->load("../assets/peri/peri.obj");
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
