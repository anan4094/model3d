//
//  Scene.cpp
//  model3d
//
//  Created by 魏裕群 on 14-4-11.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "Scene.h"

Matrix4 setFrustum(GLfloat l,GLfloat r,GLfloat b,GLfloat t,GLfloat n,GLfloat f);

void Scene::addSubNode(Node *node){
    node->setScene(this);
    m_iSubNodes.push_back(node);
}

void Scene::draw(){
    vector<Node*>::iterator itr = m_iSubNodes.begin(),end = m_iSubNodes.end();
    for (; itr!=end; itr++) {
        (*itr)->draw();
    }
}

void Scene::reshape(int w, int h){
    m_sProjection = setFrustum(-1.0f, 1.0f, -1.0f*h/w, 1.0f*h/w,2.0f, 40.0f);
}

///////////////////////////////////////////////////////////////////////////////
// glFrustum()
///////////////////////////////////////////////////////////////////////////////
Matrix4 setFrustum(float l, float r, float b, float t, float n, float f)
{
    Matrix4 mat;
    mat[0]  = 2 * n / (r - l);
    mat[2]  = (r + l) / (r - l);
    mat[5]  = 2 * n / (t - b);
    mat[6]  = (t + b) / (t - b);
    mat[10] = -(f + n) / (f - n);
    mat[11] = -(2 * f * n) / (f - n);
    mat[14] = -1;
    mat[15] = 0;
    return mat;
}
