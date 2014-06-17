//
//  Scene.cpp
//  model3d
//
//  Created by 魏裕群 on 14-4-11.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "Scene.h"
#include "Stage.h"
#include "util.h"
#include <vector>
using namespace std;

void Scene::prepareDraw(){
    glClearColor(m_fRed, m_fGreen, m_fBlue, 1);
}

void Scene::draw(){
    vector<Node*>::iterator itr = m_iSubNodes.begin(),end = m_iSubNodes.end();
    for (; itr!=end; itr++) {
        (*itr)->draw();
    }
}

void Scene::setBackgroupColor(float red,float green,float blue){
    m_fRed=red;
    m_fGreen=green;
    m_fBlue=blue;
}

void Scene::setBackgroupColor(const char *color){
    float clr[3];
    if (readColor(color, clr)) {
        m_fRed=clr[0];
        m_fGreen=clr[1];
        m_fBlue=clr[2];
    }
}

void Scene::clear(){
}

bool Scene::screenSizeChange(int width, int height){
    reshape(width, height);
    return true;
}

void Scene::reshape(int w, int h){
	glViewport(0,0,w,h);
    Stage::sharedInstance()->currentCamera()->frustum(-1.0f*w/h, 1.0f*w/h, -1.0f, 1.0f,2.0f, 40.0f);
}

void Scene::addLight(Light *light){
    m_iLights.push_back(light);
}
