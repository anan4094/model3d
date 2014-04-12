//
//  MapShader.h
//  model3d
//
//  Created by 魏裕群 on 14-4-11.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__MapShader__
#define __model3d__MapShader__

#include "Shader.h"

class MapShader:public Shader{
private:
	GLint m_nUniformModelViewProjectionMatrix;
	GLint m_nUniformSampler2D;
	MapShader();
public:
	static MapShader* sharedInstance();
	virtual void installAttrib()override;
	virtual void installUniform()override;
	void setModelViewProjectionMatrix(const float *data);
	void setTexture(int tid);
};

#endif /* defined(__model3d__MapShader__) */
