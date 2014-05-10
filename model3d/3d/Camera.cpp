//
//  camera.cpp
//  model3d
//
//  Created by 魏裕群 on 14-5-5.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "Camera.h"

///////////////////////////////////////////////////////////////////////////////
// glFrustum()
///////////////////////////////////////////////////////////////////////////////
void Camera::frustum(float l, float r, float b, float t, float n, float f){
	m_sProjection[0]  = 2 * n / (r - l);
	m_sProjection[2]  = (r + l) / (r - l);
	m_sProjection[5]  = 2 * n / (t - b);
	m_sProjection[6]  = (t + b) / (t - b);
	m_sProjection[10] = -(f + n) / (f - n);
	m_sProjection[11] = -(2 * f * n) / (f - n);
	m_sProjection[14] = -1;
	m_sProjection[15] = 0;
}

Matrix4& Camera::get(){
	return m_sProjection;
}
