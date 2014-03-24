#ifndef MODEL3D
#define MODEL3D
#ifndef __APPLE__
#include <windows.h>
#include <gl/GL.h>
#include <gl/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif
#include <vector>

typedef union vector3{
	struct{
		GLfloat x,y,z;
	};
	GLfloat d[3];
}Vector3;

typedef union texCoord{
	struct{
		GLfloat s,t,r;
	};
	struct{
		GLfloat u,v,w;
	};
	GLfloat d[3];
}TexCoord;

typedef struct face{
	int pi[3];
	int ti[3];
	int ni[3];
}Face;

class CMesh{
private:
	GLuint    m_nVBOVertices;
	GLfloat   *m_pVertices;
	GLfloat   *m_pNormals;
	GLsizei   m_nVertexCount;
    
    GLfloat   rx,ry,rz;
public:
	bool load(const char* filename);
	void draw();
    
    void setRotation(GLfloat rx,GLfloat ry,GLfloat rz);
    
	void log();
	CMesh();
	~CMesh();
};

#endif // !MODEL3D
