//
//  camera.h
//  model3d
//
//  Created by 魏裕群 on 14-5-5.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__camera__
#define __model3d__camera__
#include "platform.h"
#include "Matrices.h"
#include "Vectors.h"
class Camera{
private:
	Matrix4 m_sProjection;
public:
	void frustum(GLfloat l,GLfloat r,GLfloat b,GLfloat t,GLfloat n,GLfloat f);
	Matrix4& get();
};

#endif /* defined(__model3d__camera__) */
