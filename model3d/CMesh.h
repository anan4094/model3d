#ifndef MODEL3D
#define MODEL3D
#ifndef __APPLE__
#include <windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#else
#include <glew.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif
#include <vector>
#include "NormalShader.h"
#include "Matrices.h"
#include "Vectors.h"
#include "CMaterial.h"

#define M_PI       3.14159265358979323846

/*typedef union vector3{
	struct{
		GLfloat x,y,z;
	};
	GLfloat d[3];
}Vector3;*/

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

typedef struct mtlInfo{
    char name[32];
    GLuint first;
    GLuint size;
}MtlInfo;

class CMesh{
private:
	GLuint    m_nVBOVertices;
    GLuint    m_nVBONormals;
    GLuint    m_vertexArray;
	GLfloat   *m_pVertices;
	GLfloat   *m_pNormals;
	GLsizei   m_nVertexCount;
    
    GLfloat   rax,ray,raz,radian;
    GLfloat   x,y,z;
    GLfloat   scalex,scaley,scalez;
    
    CMaterial m_iMaterial;
    std::vector<MtlInfo> m_iMaterialArray;
    NormalShader *normalShader;
public:
	bool load(const char* filename);
	bool loadObj_bak(const char* filename);
	bool loadObj(const char* filename);
    int initShader();
	void draw();
    
    inline void setRotationAxis(GLfloat rx,GLfloat ry,GLfloat rz){GLfloat len=sqrt(rx*rx+ry*ry+rz*rz); rax=rx/len;ray=ry/len;raz=rz/len;}
    inline void setRadian(GLfloat rad){radian=rad;}
    inline void setDegree(GLfloat deg){radian=deg*M_PI/180;}
    inline void setPosition(GLfloat x,GLfloat y,GLfloat z){this->x=x;this->y=y;this->z=z;};
    inline void setScale(GLfloat x,GLfloat y,GLfloat z){scalex=x;scaley=y;scalez=z;}
	CMesh();
    CMesh(const char *name);
	~CMesh();
};
void getFilePath(const char *filename,char*childfilename);
Matrix4 setFrustum(GLfloat l,GLfloat r,GLfloat b,GLfloat t,GLfloat n,GLfloat f);

#endif // !MODEL3D
