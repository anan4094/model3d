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

#ifndef M_PI
#define M_PI       3.14159265358979323846f
#endif




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
	GLuint    m_nVerticesID;
    GLuint    m_nNormalsID;
	GLuint    m_nTexcoordsID;

    GLuint    m_nVertexArraysID;

	GLfloat   *m_pVertices;
	GLfloat   *m_pNormals;
	GLfloat   *m_pTexCoords;
	GLsizei   m_nVertexCount;
    
    GLfloat   rax,ray,raz,radian;
    GLfloat   x,y,z;
    GLfloat   scalex,scaley,scalez;
	bool      m_bHasNormal;
	bool      m_bHasTexCoord;
    bool      m_bForceGenerateNormal;
    bool      m_bSmoothSurface;
    
    CMaterial m_iMaterial;
    std::vector<MtlInfo> m_iMaterialArray;
    NormalShader *normalShader;
public:
    //加载模型
	bool load(const char* filename);
    //加载obj模型（第一版 逻辑较清晰但速度慢）
	bool loadObj_bak(const char* filename);
    //加载obj模型（第二版 速度快)
	bool loadObj(const char* filename);
    //使用glsl着色器 不调用则不使用glsl着色
    int initShader();
    
    //绘制模型
	void draw();
    
    
    inline void setRotationAxis(GLfloat rx,GLfloat ry,GLfloat rz){GLfloat len=sqrt(rx*rx+ry*ry+rz*rz); rax=rx/len;ray=ry/len;raz=rz/len;}
    inline void setRadian(GLfloat rad){radian=rad;}
    inline void setDegree(GLfloat deg){radian=deg*M_PI/180;}
    inline void setPosition(GLfloat x,GLfloat y,GLfloat z){this->x=x;this->y=y;this->z=z;};
    inline void setScale(GLfloat x,GLfloat y,GLfloat z){scalex=x;scaley=y;scalez=z;}
    //设置当模型没有法线时是否自动生成法线
    inline void setForceGenerateNormal(bool fgn){m_bForceGenerateNormal = fgn;}
    //设置当自己生成法线时是否光滑表面
    inline void setSmoothSurface(bool ss){m_bSmoothSurface = ss;}
    
	CMesh();
    CMesh(const char *name);
	~CMesh();
};
void getFilePath(const char *filename,char*childfilename);
Matrix4 setFrustum(GLfloat l,GLfloat r,GLfloat b,GLfloat t,GLfloat n,GLfloat f);

#endif // !MODEL3D
