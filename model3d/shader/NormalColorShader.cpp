//
//  NormalColorShader.cpp
//  model3d
//
//  Created by 魏裕群 on 14-3-30.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "NormalColorShader.h"

static NormalColorShader* instance = nullptr;

NormalColorShader::NormalColorShader(){
}

 NormalColorShader* NormalColorShader::sharedInstance(){
     if (instance==nullptr) {
        instance = new NormalColorShader();
		if(!instance->readShaderSource(SHADER_PATH "normal_color/shader")){
			delete instance;
			instance = nullptr;
		}else if (!instance->build()) {
            delete instance;
            instance = nullptr;
        }
    }
    return instance;
}

void NormalColorShader::installAttrib(){
    // Bind attribute locations.
    // This needs to be done prior to linking.
    glBindAttribLocation(ProgramObject,attrib_position,"position");
    glBindAttribLocation(ProgramObject,attrib_normal,"normal");
}

void NormalColorShader::installUniform(){
    m_nUniformModelViewProjectionMatrix = glGetUniformLocation(ProgramObject,"modelViewProjectionMatrix");
    m_nUniformNormalMatrix = glGetUniformLocation(ProgramObject,"normalMatrix");
    m_nUniformDiffuseColor = glGetUniformLocation(ProgramObject,"diffuseColor");
}

void NormalColorShader::setModelViewProjectionMatrix(const float *data){
    glUniformMatrix4fv(m_nUniformModelViewProjectionMatrix,1,0,data);
}
void NormalColorShader::setNormalMatrix(const float *data){
    glUniformMatrix4fv(m_nUniformNormalMatrix,1,0,data);
}
void NormalColorShader::setDiffuseColor(const float *data){
    glUniform4fv(m_nUniformDiffuseColor,1,data);
}
