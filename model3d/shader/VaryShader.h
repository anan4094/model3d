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
#include "Material.h"
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

struct materialUniform{
	GLint ka;
	GLint kd;
	GLint ks;
	GLfloat shininess;
};

class VaryShader:public Shader{
private:
    GLint m_nUniformModelViewProjectionMatrix;
	GLint m_nUniformModelViewMatrix;
    GLint m_nUniformNormalMatrix;
	GLint m_nUniformSampler2D;
	GLint m_nUniformHasmap;
	int   m_nLightCount;
	struct lightUniform m_pLightsUniform[LIGHT_MAX_NUM];
	struct materialUniform m_sMaterialUniform;
    VaryShader();
public:
    static VaryShader* sharedInstance();
    virtual void installAttrib()override;
    virtual void installUniform()override;
    void setModelViewProjectionMatrix(const float *data);
	void setModelViewMatrix(const float *data);
    void setNormalMatrix(const float *data);
	void setTexture(Texture *texture);
	void setMaterial(mtl *material);
	void setLight(int index,Light*light);
	void addLight(Light*light);
	void lightReset();
};


#endif /* defined(__model3d__VaryShader__) */
