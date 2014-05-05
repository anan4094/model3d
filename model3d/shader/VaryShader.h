//
//  VaryShader.h
//  model3d
//
//  Created by 魏裕群 on 14-5-1.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__VaryShader__
#define __model3d__VaryShader__

#include "Shader.h"
#include "Texture.h"

class VaryShader:public Shader{
private:
    GLint m_nUniformModelViewProjectionMatrix;
	GLint m_nUniformModelViewMatrix;
    GLint m_nUniformNormalMatrix;
	GLint m_nUniformSampler2D;
    VaryShader();
public:
    static VaryShader* sharedInstance();
    virtual void installAttrib()override;
    virtual void installUniform()override;
    void setModelViewProjectionMatrix(const float *data);
	void setModelViewMatrix(const float *data);
    void setNormalMatrix(const float *data);
	void setTexture(Texture *texture);
};


#endif /* defined(__model3d__VaryShader__) */
