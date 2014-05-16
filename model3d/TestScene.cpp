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
	/*m_plight->position[0]=1;
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
	m_plight->specular[3]=0;*/
	m_plight->position[0]=20;
	m_plight->position[1]=0;
	m_plight->position[2]=-20;
	m_plight->position[3]=1;
	m_plight->spotCutoff=180.0f;
	m_plight->constantAttenuation=1;
	m_plight->linearAttenuation=0.01;
	m_plight->quadraticAttenuation=0;

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

	m_plight1 = new Light();
	m_plight1->position[0]=1;
	m_plight1->position[1]=0;
	m_plight1->position[2]=0;
	m_plight1->position[3]=0;

	m_plight1->ambient[0]=.35f;
	m_plight1->ambient[1]=.35f;
	m_plight1->ambient[2]=.35f;
	m_plight1->ambient[3]=1;

	m_plight1->diffuse[0]=1;
	m_plight1->diffuse[1]=1;
	m_plight1->diffuse[2]=1;
	m_plight1->diffuse[3]=1;

	m_plight1->specular[0]=1;
	m_plight1->specular[1]=1;
	m_plight1->specular[2]=1;
	m_plight1->specular[3]=0;

	m_plight1->enabled=true;

	addLight(m_plight);
	addLight(m_plight1);

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
	scale*=.95;
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
bool TestScene::onKeyEvent(KeyEvent &event){
	//printf("key:%c\n",event.key);
	if (event.action==GLFW_PRESS){
		switch (event.key){
		case '1':
			m_plight->enabled=!m_plight->enabled;
			break;
		case '2':
			m_plight1->enabled=!m_plight1->enabled;
			break;
		case '3':
			m_plight->specular[0]=1-m_plight->specular[0];
			m_plight->specular[1]=1-m_plight->specular[1];
			m_plight->specular[2]=1-m_plight->specular[2];
			m_plight->specular[3]=0;
			break;
		case '4':
			m_plight1->specular[0]=1-m_plight1->specular[0];
			m_plight1->specular[1]=1-m_plight1->specular[1];
			m_plight1->specular[2]=1-m_plight1->specular[2];
			m_plight1->specular[3]=0;
			break;
		default:
			break;
		}
	}
	return false;
}
