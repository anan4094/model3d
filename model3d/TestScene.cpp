//
//  TestScene.cpp
//  model3d
//
//  Created by 魏裕群 on 14-4-21.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "TestScene.h"
#include "BaseAnimation.h"
#include "TranslateAnimation.h"

void TestScene::initRes(){
	m_plight = new PointLight(20,0,-20);
	//m_plight1 = new DirectionalLight(1,0,0);
	m_plight1 = new SpotLight(20);
	m_plight1->setPosition(20,4.5,-20);
	m_plight1->setDirection(-1,0,0);
	addLight(m_plight);
	addLight(m_plight1);
	
	m_pmesh=new Mesh();
	//下面两个方法要在load后加载
	m_pmesh->setForceGenerateNormal(true);
	m_pmesh->setSmoothSurface(false);
    bool (Mesh::*func)(const char*)=&Mesh::load;
#ifdef __APPLE__
	(m_pmesh->*func)("/Users/anan/Documents/github/model3d/assets/cube.obj");
#else
	m_pmesh->load("../assets/cube.obj");
#endif
	m_pmesh->setUseShader(true);
	GLfloat hy = m_pmesh->getMaxY();
	GLfloat ly = m_pmesh->getMinY();
	m_pmesh->setRotationAxis(0, 1, 0);

	//let model show us
	GLfloat scale = 18/(hy-ly);
	scale*=.2;
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
void TestScene::complete(Drawable *node){
    printf("mesh finish %p\n",this);
}
void TestScene::update(Drawable *node, float r){
    m_pmesh->radian=90*r;
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
		case '5':{
			Animation::add(m_pmesh, new TranslateAnimation(m_pmesh->x+10,m_pmesh->y,m_pmesh->z,1000))
            ->setCompleteFunction<TestScene>(this,&TestScene::complete)
            ->setUpdateFunction<TestScene>(this,&TestScene::update)->start();
			/*->setUpdateFunction([&](float r){
				m_pmesh->radian=180*r;
			})->start();*/

				 }
				 break;
		case '6':{
			Animation::add(m_pmesh, new TranslateAnimation(m_pmesh->x-10,m_pmesh->y,m_pmesh->z,1000))->start();
				 }
				 break;
		default:
			break;
		}
	}
	return false;
}
