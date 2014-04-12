#include "Texture.h"
#include "soil.h"
#include <stdio.h>

Texture::Texture():m_nTextureID(0){
	*name=0;
}

Texture::Texture(const char*filename):m_nTextureID(0){
	*name=0;
	if(load(filename)){
		printf("read picture[%s] success!\n",filename);
	}else{
		printf("read picture[%s] failed!\n",filename);
	}
}

Texture::~Texture(){
	printf("release texture%d[%s]\n",m_nTextureID,name);
	if (m_nTextureID){
		glDeleteTextures(1,&m_nTextureID);
	}
}

bool Texture::load(const char* filename){
	int i=0;
	for (const char*p=filename;*p&&i<TEXTURE_NAME_MAX_LEN-1;p++,i++){
		name[i]=*p;
	}
	name[i]=0;
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