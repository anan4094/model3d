//
//  VaryShader.cpp
//  model3d
//
//  Created by 魏裕群 on 14-5-1.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "VaryShader.h"
static VaryShader* instance = nullptr;

VaryShader::VaryShader(){
}

VaryShader* VaryShader::sharedInstance(){
    if (instance==nullptr) {
        instance = new VaryShader();
		if(!instance->readShaderSource(SHADER_PATH "vary/shader")){
			delete instance;
			instance = nullptr;
		}else if (!instance->build()) {
            delete instance;
            instance = nullptr;
        }
    }
    return instance;
}

void VaryShader::installAttrib(){
    // Bind attribute locations.
    // This needs to be done prior to linking.
    glBindAttribLocation(ProgramObject,attrib_position,"position");
    glBindAttribLocation(ProgramObject,attrib_normal,"normal");
	glBindAttribLocation(ProgramObject,attrib_texcoord0,"texcoord");
}

void VaryShader::installUniform(){
    m_nUniformModelViewProjectionMatrix = glGetUniformLocation(ProgramObject,"modelViewProjectionMatrix");
    m_nUniformNormalMatrix = glGetUniformLocation(ProgramObject,"normalMatrix");
	m_nUniformSampler2D = glGetUniformLocation(ProgramObject,"tex");
}

void VaryShader::setModelViewProjectionMatrix(const float *data){
    glUniformMatrix4fv(m_nUniformModelViewProjectionMatrix,1,0,data);
}
void VaryShader::setNormalMatrix(const float *data){
    glUniformMatrix4fv(m_nUniformNormalMatrix,1,0,data);
}

void VaryShader::setTexture(int tid){
	glUniform1i(m_nUniformSampler2D,tid);
}
