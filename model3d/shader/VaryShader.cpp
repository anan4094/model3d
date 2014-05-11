//
//  VaryShader.cpp
//  model3d
//
//  Created by 魏裕群 on 14-5-1.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "VaryShader.h"
#include <stdio.h>
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
	m_nUniformModelViewMatrix = glGetUniformLocation(ProgramObject,"modelViewMatrix");
    m_nUniformNormalMatrix = glGetUniformLocation(ProgramObject,"normalMatrix");
	m_nUniformSampler2D = glGetUniformLocation(ProgramObject,"tex");
	//GLint m_nUniformLights = glGetUniformBlockIndex(ProgramObject,"light");

	char *name=new char[40],*p=name+6,*q;
	strcpy(name,"light[");
	for (int i=0;i<LIGHT_MAX_NUM;i++){
		if (i<10){
			*p='0'+i;
			*(p+1)=']';
			*(p+2)='.';
			q=p+3;
		}else{
			*p='0'+(i/10);
			*(p+1)='0'+(i%10);
			*(p+2)=']';
			*(p+3)='.';
			q=p+4;
		}
		strcpy(q,"enabled");
		m_pLightsUniform[i].enabled=glGetUniformLocation(ProgramObject,name);
		strcpy(q,"position");
		m_pLightsUniform[i].position=glGetUniformLocation(ProgramObject,name);
		strcpy(q,"ambient");
		m_pLightsUniform[i].ambient=glGetUniformLocation(ProgramObject,name);
		strcpy(q,"diffuse");
		m_pLightsUniform[i].diffuse=glGetUniformLocation(ProgramObject,name);
		strcpy(q,"specular");
		m_pLightsUniform[i].specular=glGetUniformLocation(ProgramObject,name);
		strcpy(q,"spotDirection");
		m_pLightsUniform[i].spotDirection=glGetUniformLocation(ProgramObject,name);
		strcpy(q,"spotCutoff");
		m_pLightsUniform[i].spotCutoff=glGetUniformLocation(ProgramObject,name);
		strcpy(q,"spotExponent");
		m_pLightsUniform[i].spotExponent=glGetUniformLocation(ProgramObject,name);
		strcpy(q,"constantAttenuation");
		m_pLightsUniform[i].constantAttenuation=glGetUniformLocation(ProgramObject,name);
		strcpy(q,"linearAttenuation");
		m_pLightsUniform[i].linearAttenuation=glGetUniformLocation(ProgramObject,name);
		strcpy(q,"quadraticAttenuation");
		m_pLightsUniform[i].quadraticAttenuation=glGetUniformLocation(ProgramObject,name);
	}
	delete[] name;
}

void VaryShader::setModelViewProjectionMatrix(const float *data){
    glUniformMatrix4fv(m_nUniformModelViewProjectionMatrix,1,0,data);
}
void VaryShader::setModelViewMatrix(const float *data){
	glUniformMatrix4fv(m_nUniformModelViewMatrix,1,0,data);
}
void VaryShader::setNormalMatrix(const float *data){
    glUniformMatrix4fv(m_nUniformNormalMatrix,1,0,data);
}

void VaryShader::setTexture(Texture* texture){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,texture->get());
	glUniform1i(m_nUniformSampler2D,0);
}

void VaryShader::setLight(int index,Light*light){
	if (index<0||index>=LIGHT_MAX_NUM){
		return;
	}
	glUniform1i(m_pLightsUniform[index].enabled,light->enabled);
	glUniform4fv(m_pLightsUniform[index].ambient,1,light->ambient);
	glUniform4fv(m_pLightsUniform[index].diffuse,1,light->diffuse);
	glUniform4fv(m_pLightsUniform[index].position,1,light->position);
	glUniform4fv(m_pLightsUniform[index].specular,1,light->specular);
	glUniform3fv(m_pLightsUniform[index].spotDirection,1,light->spotDirection);
	glUniform1f(m_pLightsUniform[index].spotCutoff,light->spotCutoff);
	glUniform1f(m_pLightsUniform[index].spotExponent,light->spotExponent);
	glUniform1f(m_pLightsUniform[index].constantAttenuation,light->constantAttenuation);
	glUniform1f(m_pLightsUniform[index].linearAttenuation,light->linearAttenuation);
	glUniform1f(m_pLightsUniform[index].quadraticAttenuation,light->quadraticAttenuation);
}