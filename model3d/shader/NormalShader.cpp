//
//  normal_shader.cpp
//  model3d
//
//  Created by 魏裕群 on 14-3-30.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "NormalShader.h"

static NormalShader* instance = nullptr;

NormalShader::NormalShader(){
}

 NormalShader* NormalShader::sharedInstance(){
     if (instance==nullptr) {
        instance = new NormalShader();
		instance->readShaderSource(SHADER_PATH "normal/shader");
        if (!instance->build()) {
            delete instance;
            instance = nullptr;
        }
    }
    return instance;
}

void NormalShader::installAttrib(){
    // Bind attribute locations.
    // This needs to be done prior to linking.
    glBindAttribLocation(ProgramObject,attrib_position,"position");
    glBindAttribLocation(ProgramObject,attrib_normal,"normal");
	glBindAttribLocation(ProgramObject,attrib_texcoord0,"texcoord");
}

void NormalShader::installUniform(){
    m_nUniformModelViewProjectionMatrix = glGetUniformLocation(ProgramObject,"modelViewProjectionMatrix");
    m_nUniformNormalMatrix = glGetUniformLocation(ProgramObject,"normalMatrix");
    m_nUniformDiffuseColor = glGetUniformLocation(ProgramObject,"diffuseColor");
	m_nUniformSampler2D = glGetUniformLocation(ProgramObject,"tex");
}

void NormalShader::setModelViewProjectionMatrix(const float *data){
    glUniformMatrix4fv(m_nUniformModelViewProjectionMatrix,1,0,data);
}
void NormalShader::setNormalMatrix(const float *data){
    glUniformMatrix4fv(m_nUniformNormalMatrix,1,0,data);
}
void NormalShader::setDiffuseColor(const float *data){
    glUniform4fv(m_nUniformDiffuseColor,1,data);
}

void NormalShader::setTexture(int tid){
	glUniform1i(m_nUniformSampler2D,tid);
}