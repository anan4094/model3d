//
//  NormalMapShader.h
//  model3d
//
//  Created by 魏裕群 on 14-3-30.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__normal_map_shader__
#define __model3d__normal_map_shader__
#include "Shader.h"
#include "Texture.h"

class NormalMapShader:public Shader{
private:
    GLint m_nUniformModelViewProjectionMatrix;
    GLint m_nUniformNormalMatrix;
	GLint m_nUniformSampler2D;
    NormalMapShader();
public:
    static NormalMapShader* sharedInstance();
    virtual void installAttrib()override;
    virtual void installUniform()override;
    void setModelViewProjectionMatrix(const float *data);
    void setNormalMatrix(const float *data);
	void setTexture(Texture *texture);
};

#endif /* defined(__model3d__normal_map_shader__) */
