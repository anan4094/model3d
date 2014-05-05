//
//  NormalMapShader.cpp
//  model3d
//
//  Created by 魏裕群 on 14-3-30.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "NormalMapShader.h"

static NormalMapShader* instance = nullptr;

NormalMapShader::NormalMapShader(){
}

 NormalMapShader* NormalMapShader::sharedInstance(){
     if (instance==nullptr) {
        instance = new NormalMapShader();
		if(!instance->readShaderSource(SHADER_PATH "normal_map/shader")){
			delete instance;
			instance = nullptr;
		}else if (!instance->build()) {
            delete instance;
            instance = nullptr;
        }
    }
    return instance;
}

void NormalMapShader::installAttrib(){
    // Bind attribute locations.
    // This needs to be done prior to linking.
    glBindAttribLocation(ProgramObject,attrib_position,"position");
    glBindAttribLocation(ProgramObject,attrib_normal,"normal");
	glBindAttribLocation(ProgramObject,attrib_texcoord0,"texcoord");
}

void NormalMapShader::installUniform(){
    m_nUniformModelViewProjectionMatrix = glGetUniformLocation(ProgramObject,"modelViewProjectionMatrix");
    m_nUniformNormalMatrix = glGetUniformLocation(ProgramObject,"normalMatrix");
	m_nUniformSampler2D = glGetUniformLocation(ProgramObject,"tex");
}

void NormalMapShader::setModelViewProjectionMatrix(const float *data){
    glUniformMatrix4fv(m_nUniformModelViewProjectionMatrix,1,0,data);
}
void NormalMapShader::setNormalMatrix(const float *data){
    glUniformMatrix4fv(m_nUniformNormalMatrix,1,0,data);
}

void NormalMapShader::setTexture(Texture* texture){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,texture->get());
	glUniform1i(m_nUniformSampler2D,0);
}