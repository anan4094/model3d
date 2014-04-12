#ifndef __model3d__texture__
#define __model3d__texture__

#include "platform.h"

#define TEXTURE_NAME_MAX_LEN 64

class Texture
{
private:
	GLuint m_nTextureID;
	char name[TEXTURE_NAME_MAX_LEN];
public:
	Texture();
	Texture(const char*filename);
	~Texture();
	inline const char* getName(){return name;}
	bool load(const char*filename);
	inline GLuint get(){return m_nTextureID;}
};

#endif // !__model3d__texture__
