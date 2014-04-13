//
//  NormalColorShader.h
//  model3d
//
//  Created by 魏裕群 on 14-3-30.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__normal_color_shader__
#define __model3d__normal_color_shader__
#include "Shader.h"

class NormalColorShader:public Shader{
private:
    GLint m_nUniformModelViewProjectionMatrix;
    GLint m_nUniformNormalMatrix;
    GLint m_nUniformDiffuseColor;
    NormalColorShader();
public:
    static NormalColorShader* sharedInstance();
    virtual void installAttrib()override;
    virtual void installUniform()override;
    void setModelViewProjectionMatrix(const float *data);
    void setNormalMatrix(const float *data);
    void setDiffuseColor(const float *data);
};

#endif /* defined(__model3d__normal_color_shader__) */
