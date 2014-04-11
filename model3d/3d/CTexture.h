#ifndef __model3d__texture__
#define __model3d__texture__

#include "platform.h"

class CTexture
{
private:
	GLuint m_nTextureID;
public:
	CTexture();
	CTexture(const char*filename);
	~CTexture();
	bool load(const char*filename);
	inline GLuint get(){return m_nTextureID;}
};

#endif // !__model3d__texture__
