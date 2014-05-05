//
//  MapShader.cpp
//  model3d
//
//  Created by 魏裕群 on 14-4-11.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "MapShader.h"
static MapShader* instance = nullptr;

MapShader::MapShader(){
}

MapShader* MapShader::sharedInstance(){
	if (instance==nullptr) {
		instance = new MapShader();
		if(!instance->readShaderSource(SHADER_PATH "map/shader")){
			delete instance;
			instance = nullptr;
		}else if (!instance->build()) {
			delete instance;
			instance = nullptr;
		}
	}
	return instance;
}

void MapShader::installAttrib(){
	// Bind attribute locations.
	// This needs to be done prior to linking.
	glBindAttribLocation(ProgramObject,attrib_position,"position");
	glBindAttribLocation(ProgramObject,attrib_texcoord0,"texcoord");
}

void MapShader::installUniform(){
	m_nUniformModelViewProjectionMatrix = glGetUniformLocation(ProgramObject,"modelViewProjectionMatrix");
	m_nUniformSampler2D = glGetUniformLocation(ProgramObject,"tex");
}

void MapShader::setModelViewProjectionMatrix(const float *data){
	glUniformMatrix4fv(m_nUniformModelViewProjectionMatrix,1,0,data);
}

void MapShader::setTexture(Texture* texture){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,texture->get());
	glUniform1i(m_nUniformSampler2D,0);
}