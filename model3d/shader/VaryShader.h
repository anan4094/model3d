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
#include "Light.h"

#define LIGHT_MAX_NUM 36

struct lightUniform{
	GLint enabled;
	GLint position;
	GLint ambient;
	GLint diffuse;
	GLint specular;
	GLint spotDirection;
	GLint spotCutoff;
	GLint spotExponent;
	GLint constantAttenuation;
	GLint linearAttenuation;
	GLint quadraticAttenuation;
};

class VaryShader:public Shader{
private:
    GLint m_nUniformModelViewProjectionMatrix;
	GLint m_nUniformModelViewMatrix;
    GLint m_nUniformNormalMatrix;
	GLint m_nUniformSampler2D;
	struct lightUniform m_pLightsUniform[LIGHT_MAX_NUM];
    VaryShader();
public:
    static VaryShader* sharedInstance();
    virtual void installAttrib()override;
    virtual void installUniform()override;
    void setModelViewProjectionMatrix(const float *data);
	void setModelViewMatrix(const float *data);
    void setNormalMatrix(const float *data);
	void setTexture(Texture *texture);
	void setLight(int index,Light*light);
};


#endif /* defined(__model3d__VaryShader__) */
