#include "CTexture.h"
#include "soil/soil.h"
#include <stdio.h>

CTexture::CTexture():m_nTextureID(0){
}

CTexture::CTexture(const char*filename){
	CTexture();
	if(load(filename)){
		printf("read picture[%s] success!\n",filename);
	}else{
		printf("read picture[%s] failed!\n",filename);
	}
}

CTexture::~CTexture(){
}

bool CTexture::load(const char* filename){
	m_nTextureID = SOIL_load_OGL_texture  
		(  
		filename,  
		SOIL_LOAD_AUTO,  
		SOIL_CREATE_NEW_ID,  
		SOIL_FLAG_INVERT_Y  
		);
	if(m_nTextureID == 0)  
		return false;
	// Typical Texture Generation Using Data From The Bitmap  
	glBindTexture(GL_TEXTURE_2D, m_nTextureID);  
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	return true;
}