#ifndef __model3d__texture__
#define __model3d__texture__

#ifndef __APPLE__
#include <windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#else
#include <glew.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif

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
